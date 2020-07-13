#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_deferred_resolve()
{
	bool	_menu_pp	= g_pGamePersistent?g_pGamePersistent->OnRenderPPUI_query():false;

	CEnvDescriptorMixer& environment = g_pGamePersistent->Environment().CurrentEnv;

	// Set RT for sky/clouds and deferred resolve
	u_setrt	(rt_Generic_0, 0, 0, 0, HW.pBaseZB);
	RCache.set_CullMode( CULL_NONE );
	RCache.set_Stencil(FALSE);

	/*u_setrt(Device.dwWidth, Device.dwHeight, HW.pBaseRT, NULL, NULL,NULL, HW.pBaseZB);
	RCache.set_CullMode(CULL_NONE);
	RCache.set_Stencil(FALSE);*/

	BOOL split_the_scene_to_minimize_wait = FALSE;
	if (ps_r2_ls_flags.test(R2FLAG_EXP_SPLIT_SCENE))
		split_the_scene_to_minimize_wait = TRUE;

	// Draw Sky and Clouds
	if (1)
	{
		RCache.set_ColorWriteEnable();
		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, FALSE));
		g_pGamePersistent->Environment().RenderSky();
		g_pGamePersistent->Environment().RenderClouds();
		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, TRUE));

		// Mask
		RCache.set_Stencil(TRUE, D3DCMP_LESSEQUAL, 0x01, 0xff, 0x00);
	}
	else 
		CHK_DX(HW.pDevice->Clear(0L, NULL, D3DCLEAR_TARGET, color_rgba(0, 127, 255, 127), 1.0f, 0L));

	if (!_menu_pp)
	{
		u32	Offset = 0;
		Fvector2 p0, p1;

		struct v_deferredResolve
		{
			Fvector4 position;
			Fvector2 uv0;
		};

		float width = float(Device.dwWidth);
		float height = float(Device.dwHeight);

		float du = 0.5f / width;
		float dv = 0.5f / height;

		p0.set(du, dv);
		p1.set((width + 0.5f) / width, (height + 0.5f) / height);

		// Fill vertex buffer
		v_deferredResolve* pv = (v_deferredResolve*) RCache.Vertex.Lock(4, g_combine_VP->vb_stride, Offset);
		pv->position.set(EPS, float(height + EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->position.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->position.set(float(width + EPS), float(height + EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->position.set(float(width + EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;

		RCache.Vertex.Unlock(4, g_combine_VP->vb_stride);

		// Draw
		RCache.set_Element(s_combine->E[0]);
		RCache.set_Geometry(g_combine_VP);

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

		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}


	if (_menu_pp)			
	{
		CHK_DX(HW.pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, 0,	1.0f, 0L));
	};
}