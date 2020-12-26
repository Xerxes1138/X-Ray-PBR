#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_postprocess_AO ()
{
	// Raycast
	if(1)
	{
		CEnvDescriptorMixer& environment = g_pGamePersistent->Environment().CurrentEnv;

		u32			Offset = 0;
		Fvector2	p0,p1;

		struct v_postprocess_AO
		{
			Fvector4	position;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth) / 2.0f;
		float	_h					= float(Device.dwHeight) / 2.0f;

		float du = 0.5f / _w;
		float dv = 0.5f / _h;

		p0.set(du, dv);
		p1.set((_w + 0.5f) / _w, (_h + 0.5f) / _h);

		// Fill vertex buffer
		v_postprocess_AO* pv = (v_postprocess_AO*) RCache.Vertex.Lock(4, g_postprocess_AO->vb_stride, Offset);
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
		
		RCache.Vertex.Unlock(4, g_postprocess_AO->vb_stride);

		RCache.set_Shader	(s_postprocess_AO);

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

		Fvector4 SSAOParams = {ps_r2_ssao_length, ps_r2_ssao_thickness, ps_r2_ssao_size, ps_r2_ssgi_temporal_response};
		RCache.set_c("dx_SSAO_Params", SSAOParams);

		Fvector4 sundir;
		{
			light*		fuckingsun		= (light*)RImplementation.Lights.sun_adapted._get()	;
			Fvector		L_dir;	
			//L_dir = fuckingsun->direction; // World space light direction
			Device.mView.transform_dir	(L_dir,fuckingsun->direction);  // View space light direction
			L_dir.normalize				();
			sundir.set				(L_dir.x,L_dir.y,L_dir.z,0);
		}

		RCache.set_c				("Ldynamic_dir",	sundir	);

		u_setrt	(rt_AO, NULL, NULL, NULL, HW.pBaseZB);
		RCache.set_CullMode(CULL_NONE);
		RCache.set_Stencil(FALSE);
		RCache.set_ColorWriteEnable	();
		RCache.set_Geometry(g_postprocess_AO);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
		RCache.set_ColorWriteEnable	(FALSE);
	}

	// Resolve
	if(1)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		struct v_postprocess_ssao_resolve
		{
			Fvector4	position;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		float du = 0.5f / _w;
		float dv = 0.5f / _h;

		p0.set(du, dv);
		p1.set((_w + 0.5f) / _w, (_h + 0.5f) / _h);

		// Fill vertex buffer
		v_postprocess_ssao_resolve* pv = (v_postprocess_ssao_resolve*) RCache.Vertex.Lock(4, g_postprocess_AO_resolve->vb_stride, Offset);
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
		
		RCache.Vertex.Unlock(4, g_postprocess_AO_resolve->vb_stride);

		RCache.set_Shader	(s_postprocess_AO_resolve);

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

		Fvector4 SSAOParams = {ps_r2_ssao_length, ps_r2_ssao_thickness, ps_r2_ssao_size, ps_r2_ssgi_temporal_response};
		RCache.set_c("dx_SSAO_Params", SSAOParams);

		u_setrt	(rt_AO_Resolve, NULL, NULL, NULL, HW.pBaseZB);
		RCache.set_ColorWriteEnable	();
		RCache.set_CullMode( CULL_NONE );
		RCache.set_Stencil(FALSE);

		RCache.set_Geometry			(g_postprocess_AO_resolve);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}

	// Temporal
	if(1)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_AO_Resolve, 0, 0, 0, HW.pBaseZB);
		RCache.set_CullMode(CULL_NONE);
		RCache.set_Stencil(FALSE);

		struct v_postprocess_AO_temporal
		{
			Fvector4	position;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		float du = 0.5f / _w;
		float dv = 0.5f / _h;

		p0.set(du, dv);
		p1.set((_w + 0.5f) / _w, (_h + 0.5f) / _h);

		// Fill vertex buffer
		v_postprocess_AO_temporal* pv = (v_postprocess_AO_temporal*) 
		
		RCache.Vertex.Lock(4, g_postprocess_AO_temporal->vb_stride, Offset);

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

		RCache.Vertex.Unlock(4, g_postprocess_AO_temporal->vb_stride);

		RCache.set_Shader	(s_postprocess_AO_temporal);
		Fmatrix	ViewMatrix; ViewMatrix.set(Device.mView);
		Fmatrix	ProjectioMatrix; ProjectioMatrix.set(Device.mProjectUnJittered);
		Fmatrix	InverseProjectioMatrix; InverseProjectioMatrix.invert(Device.mProjectUnJittered);

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

		Fvector4 taaJitter;
		taaJitter.x = Device.vTAAJitter.x;
		taaJitter.y = Device.vTAAJitter.y;
		taaJitter.z = 0;
		taaJitter.w = 0;

		RCache.set_c				("dx_TAA_Jitter", taaJitter);

		Fvector4 SSAOParams = {ps_r2_ssao_length, ps_r2_ssao_thickness, ps_r2_ssao_size, ps_r2_ssgi_temporal_response};
		RCache.set_c("dx_SSAO_Params", SSAOParams);

		RCache.set_Geometry			(g_postprocess_AO_temporal);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}

	if(1)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_AO_Temporal, 0,0,0, HW.pBaseZB );
		RCache.set_CullMode(CULL_NONE);
		RCache.set_Stencil(FALSE);

		struct v_blit_ssao	
		{
			Fvector4	p;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_blit_ssao* pv = (v_blit_ssao*)
			
		RCache.Vertex.Lock(4, g_blit_ssao->vb_stride, Offset);

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

		RCache.Vertex.Unlock(4, g_blit_ssao->vb_stride);

		RCache.set_Shader(s_blit_ssao);
		RCache.set_Geometry	(g_blit_ssao);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}
}