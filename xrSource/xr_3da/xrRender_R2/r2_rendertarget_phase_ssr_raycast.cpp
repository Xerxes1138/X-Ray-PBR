#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_ssr_raycast ()
{
	if(1)
	{
		CEnvDescriptorMixer& environment = g_pGamePersistent->Environment().CurrentEnv;

		u32			Offset = 0;
		Fvector2	p0,p1;

		u_setrt	(rt_SSR_Raycast, NULL, NULL, NULL, HW.pBaseZB);
		//CHK_DX(HW.pDevice->Clear	( 0L, NULL, D3DCLEAR_TARGET, 0x0, 1.0f, 0L));
		RCache.set_CullMode( CULL_NONE );
		RCache.set_Stencil(FALSE);

		struct v_postprocess_ssr
		{
			Fvector4	position;
			Fvector2	uv0;
		};

		BOOL raycastHalf = ps_r2_ssr_flags.test(R2FLAG_SSR_RAYCAST_HALF);

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		if(raycastHalf)
		{
			_w	/= 2;
			_h	/= 2;
		}

		float du = 0.5f / _w;
		float dv = 0.5f / _h;

		p0.set(du, dv);
		p1.set((_w + 0.5f) / _w, (_h + 0.5f) / _h);

		// Fill vertex buffer
		v_postprocess_ssr* pv = (v_postprocess_ssr*) RCache.Vertex.Lock(4, g_postprocess_ssr->vb_stride, Offset);
		pv->position.set(EPS, float(_h + EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->position.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->position.set(float(_w + EPS), float(_h + EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->position.set(float(_w + EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;
		
		RCache.Vertex.Unlock(4, g_postprocess_ssr->vb_stride);

		RCache.set_Shader	(s_postprocess_ssr);

		Fvector4 screenSize = {_w, _h, 1.0f / _w, 1.0f / _h};

		RCache.set_c("dx_ScreenSize",	screenSize);

		Fmatrix	ViewMatrix; ViewMatrix.set(Device.mView);
		Fmatrix	ProjectioMatrix; ProjectioMatrix.set(Device.mProject);
		Fmatrix	InverseProjectioMatrix; InverseProjectioMatrix.invert(Device.mProject);

		Fmatrix	ViewProjectionMatrix;
		ViewProjectionMatrix.mul(ViewMatrix, ProjectioMatrix);

		Fmatrix	InverseViewProjectionMatrix;
		InverseViewProjectionMatrix.invert(ViewProjectionMatrix);

		Fmatrix	CameraToWorldMatrix; CameraToWorldMatrix.invert(Device.mView);

		// TODO : Move this to standard binding
		RCache.set_c("dx_matrix_Projection", ProjectioMatrix);
		RCache.set_c("dx_matrix_CameraToWorld", CameraToWorldMatrix);
		RCache.set_c("dx_matrix_InverseProjection", InverseProjectioMatrix);
		RCache.set_c("dx_matrix_ViewProjection", ViewProjectionMatrix);
		RCache.set_c("dx_matrix_InverseViewProjection",	InverseViewProjectionMatrix);

		RCache.set_Geometry			(g_postprocess_ssr);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}
}