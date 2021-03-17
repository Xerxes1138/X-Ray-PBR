#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

float	hclipTAA(float v, float dim)		{ return 2.f*v/dim - 1.f; }

void CRenderTarget::phase_postprocess_TAA()
{	
	if(RImplementation.o.temporalAA)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_currentColor, 0, 0, 0, 0);

		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, FALSE));

		struct v_postprocess_taa
		{
			Fvector4	position;
			Fvector2	uv0;
			Fvector2	uv1;
		};

		float width = float(Device.dwWidth);
		float height = float(Device.dwHeight);

		float du = 0.5f / width;
		float dv = 0.5f / height;

		p0.set(du, dv);
		p1.set((width + 0.5f) / width, (height + 0.5f) / height);

		// Fill vertex buffer
		v_postprocess_taa* pv = (v_postprocess_taa*)
		
		RCache.Vertex.Lock(4, g_postprocess_taa->vb_stride, Offset);

		pv->position.set(EPS, float(height + EPS), EPS, 1.0f);
		pv->uv0.set(p0.x, p1.y);
		pv->uv1.set(p0.x, p1.y);
		pv++;

		pv->position.set(EPS, EPS,	EPS, 1.0f);
		pv->uv0.set(p0.x, p0.y);
		pv->uv1.set(p0.x, p0.y);
		pv++;

		pv->position.set(float(width + EPS), float(height + EPS), EPS, 1.0f); 
		pv->uv0.set(p1.x, p1.y);
		pv->uv1.set(p1.x, p1.y);
		pv++;

		pv->position.set(float(width + EPS), EPS, EPS, 1.0f); 
		pv->uv0.set(p1.x, p0.y); 
		pv->uv1.set(p1.x, p0.y);
		pv++;

		RCache.Vertex.Unlock(4, g_postprocess_taa->vb_stride);

		RCache.set_Shader	(s_postprocess_taa);
		Fmatrix	ViewMatrix; ViewMatrix.set(Device.mView);
		Fmatrix	ProjectioMatrix; ProjectioMatrix.set(Device.mProject);
		Fmatrix	InverseProjectioMatrix; InverseProjectioMatrix.invert(Device.mProject);

		Fmatrix	ViewProjectionMatrix;
		ViewProjectionMatrix.mul(ViewMatrix, ProjectioMatrix);

		Fmatrix	InverseViewProjectionMatrix;
		InverseViewProjectionMatrix.invert(ViewProjectionMatrix);

		Fmatrix	CameraToWorldMatrix; CameraToWorldMatrix.invert(Device.mView);

		RCache.set_c("dx_matrix_Projection", ProjectioMatrix);
		RCache.set_c("dx_matrix_CameraToWorld", CameraToWorldMatrix);
		RCache.set_c("dx_matrix_InverseProjection", InverseProjectioMatrix);
		RCache.set_c("dx_matrix_ViewProjection", ViewProjectionMatrix);
		RCache.set_c("dx_matrix_InverseViewProjection",	InverseViewProjectionMatrix);

		Fvector4 taaJitter;
		taaJitter.x = Device.vTAAJitter.x;
		taaJitter.y = Device.vTAAJitter.y;
		taaJitter.z = 0;
		taaJitter.w = 0;

		RCache.set_c				("dx_TAA_Jitter", taaJitter);

		Fvector4 TAAParams = {ps_r2_taa_min_response, ps_r2_taa_max_response, 0, 0};
		RCache.set_c("dx_TAA_Params", TAAParams);

		RCache.set_Geometry			(g_postprocess_taa);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
		
		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, TRUE));
	}

	if(RImplementation.o.temporalAA)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_previousColor, 0,0,0,0 );

		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, FALSE));

		struct v_blit_taa	
		{
			Fvector4	p;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_blit_taa* pv = (v_blit_taa*) RCache.Vertex.Lock(4, g_blit_taa->vb_stride, Offset);
		pv->p.set(EPS, float(_h+EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->p.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->p.set(float(_w+EPS), float(_h+EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->p.set(float(_w+EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;

		RCache.Vertex.Unlock(4, g_blit_taa->vb_stride);

		RCache.set_Shader(s_blit_taa);
		RCache.set_Geometry	(g_blit_taa);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);

		CHK_DX(HW.pDevice->SetRenderState(D3DRS_ZENABLE, TRUE));
	}

	// Recursive pass
	if(0)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_SSGI_Combine, 0,0,0, HW.pBaseZB );
		RCache.set_CullMode(CULL_NONE);
		RCache.set_Stencil(FALSE);

		struct v_blit_ssgi_recursive	
		{
			Fvector4	p;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_blit_ssgi_recursive* pv = (v_blit_ssgi_recursive*)
			
		RCache.Vertex.Lock(4, g_blit_ssgi->vb_stride, Offset);

		pv->p.set(EPS, float(_h+EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->p.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->p.set(float(_w+EPS), float(_h+EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->p.set(float(_w+EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;

		RCache.Vertex.Unlock(4, g_blit_ssgi_recursive->vb_stride);

		RCache.set_Shader(s_blit_ssgi_recursive);
		RCache.set_Geometry	(g_blit_ssgi_recursive);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}
}