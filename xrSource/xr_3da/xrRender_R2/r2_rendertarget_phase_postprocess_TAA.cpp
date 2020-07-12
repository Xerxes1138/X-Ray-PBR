#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

float	hclipTAA(float v, float dim)		{ return 2.f*v/dim - 1.f; }

void CRenderTarget::phase_postprocess_TAA()
{	
	if (1)	
	{
		//phase_copy (rt_Generic_1, rt_previousColor);

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

		/*Fvector4* pv				= (Fvector4*)

		RCache.Vertex.Lock	(4,g_postprocess_taa->vb_stride,Offset);

		pv->set						(hclipTAA(EPS,		width),	hclipTAA(height+EPS,	height),	p0.x, p1.y);	pv++;
		pv->set						(hclipTAA(EPS,		width),	hclipTAA(EPS,		height),	p0.x, p0.y);	pv++;
		pv->set						(hclipTAA(width+EPS,	width),	hclipTAA(height+EPS,	height),	p1.x, p1.y);	pv++;
		pv->set						(hclipTAA(width+EPS,	width),	hclipTAA(EPS,		height),	p1.x, p0.y);	pv++;

		RCache.Vertex.Unlock(4, g_postprocess_taa->vb_stride);*/

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

		phase_copy (rt_previousColor, rt_previousColor);
	}
}