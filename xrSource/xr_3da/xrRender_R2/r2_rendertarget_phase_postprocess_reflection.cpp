#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::phase_postprocess_reflection()
{
	CEnvDescriptorMixer& environment = g_pGamePersistent->Environment().CurrentEnv;

	// SSR + IBL Spec + FOG
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_Generic_1, NULL, NULL, NULL,  HW.pBaseZB);
		RCache.set_CullMode( CULL_NONE );
		RCache.set_Stencil(FALSE);

		struct v_postprocess_reflection
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
		v_postprocess_reflection* pv = (v_postprocess_reflection*) RCache.Vertex.Lock(4, g_postprocess_reflection->vb_stride, Offset);
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
		
		RCache.Vertex.Unlock(4, g_postprocess_reflection->vb_stride);

		RCache.set_Shader	(s_postprocess_reflection);

		Fmatrix	ViewMatrix; ViewMatrix.set(Device.mView);
		Fmatrix	ProjectioMatrix; ProjectioMatrix.set(Device.mProject);
		Fmatrix	InverseProjectioMatrix; InverseProjectioMatrix.invert(Device.mProject);

		Fmatrix	ViewProjectionMatrix;
		ViewProjectionMatrix.mul(ViewMatrix, ProjectioMatrix);

		Fmatrix	InverseViewProjectionMatrix;
		InverseViewProjectionMatrix.invert(ViewProjectionMatrix);

		Fmatrix	CameraToWorldMatrix; CameraToWorldMatrix.invert(Device.mView);

		Fvector4 ambientColor = {environment.ambient.x,	environment.ambient.y, environment.ambient.z, 1.0f};
		ambientColor.x *= ps_r2_sun_lumscale_amb;
		ambientColor.y *= ps_r2_sun_lumscale_amb;
		ambientColor.z *= ps_r2_sun_lumscale_amb;

		Fvector4 skyColor = {environment.sky_color.x, environment.sky_color.y, environment.sky_color.z, environment.weight};
		skyColor.x *= ps_r2_sun_lumscale_hemi; 
		skyColor.y *= ps_r2_sun_lumscale_hemi; 
		skyColor.z *= ps_r2_sun_lumscale_hemi;

		Fvector4 skyParams = {environment.sky_rotation, 0, 0, 0};

		RCache.set_c("dx_matrix_Projection", ProjectioMatrix);
		RCache.set_c("dx_matrix_CameraToWorld", CameraToWorldMatrix);
		RCache.set_c("dx_matrix_InverseProjection", InverseProjectioMatrix);
		RCache.set_c("dx_matrix_ViewProjection", ViewProjectionMatrix);
		RCache.set_c("dx_matrix_InverseViewProjection",	InverseViewProjectionMatrix);

		RCache.set_c("dx_SkyRotation", skyParams);
		RCache.set_c("dx_AmbientColor", ambientColor);
		RCache.set_c("dx_SkyColor", skyColor);

		RCache.set_c				("L_ambient",		ambientColor	);

		Fvector4	fogclr			= {environment.fog_color.x,	environment.fog_color.y, environment.fog_color.z, 0.0f};

		Fvector4	sunclr,sundir;

		// sun-params
		{
			light*		fuckingsun		= (light*)RImplementation.Lights.sun_adapted._get()	;
			Fvector		L_dir,L_clr;	float L_spec;
			L_clr.set					(fuckingsun->color.r,fuckingsun->color.g,fuckingsun->color.b);
			L_spec						= u_diffuse2s	(L_clr);

			//L_dir = fuckingsun->direction; // World space light direction
			Device.mView.transform_dir	(L_dir,fuckingsun->direction);  // View space light direction
			L_dir.normalize				();

		if(1)
		{
			L_clr.x = powf(L_clr.x, 2.2f);
			L_clr.y = powf(L_clr.y, 2.2f);
			L_clr.z = powf(L_clr.z, 2.2f);
		}

			sunclr.set				(L_clr.x,L_clr.y,L_clr.z,L_spec);
			sundir.set				(L_dir.x,L_dir.y,L_dir.z,0);
		}

		RCache.set_c				("Ldynamic_color",	sunclr	);
		RCache.set_c				("Ldynamic_dir",	sundir	);

		RCache.set_c				("env_color",		skyColor	);
		RCache.set_c				("fog_color",		fogclr	);

		Fvector4 SSRParams = {ps_r2_ssr_temporal_response, ps_r2_ssr_atten_border, 0, 0};
		RCache.set_c("dx_SSR_Params", SSRParams);

		RCache.set_Geometry			(g_postprocess_reflection);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}

	if(1)
	{
		
	// Previous matrix
	Fmatrix		m_previous, m_current;
	//Fvector2	m_blur_scale;
	//{
		static Fmatrix		m_saved_viewproj;
		
		// (new-camera) -> (world) -> (old_viewproj)
		Fmatrix	m_invview;
		m_invview.invert(Device.mView);

		m_previous.mul(m_saved_viewproj, m_invview);

		m_current.set(Device.mProjectUnJittered);

		m_saved_viewproj.mul(Device.mProjectUnJittered, Device.mView);

		//float	scale		= ps_r2_mblur / 2.f;
		//m_blur_scale.set	(scale, -scale).div(12.f);
	//}

		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_ssr_reflection, 0, 0, 0, HW.pBaseZB);
		RCache.set_CullMode(CULL_NONE);
		RCache.set_Stencil(FALSE);

		struct v_postprocess_ssr_temporal
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
		v_postprocess_ssr_temporal* pv = (v_postprocess_ssr_temporal*) 
		
		RCache.Vertex.Lock(4, g_postprocess_ssr_temporal->vb_stride, Offset);

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

		RCache.Vertex.Unlock(4, g_postprocess_ssr_temporal->vb_stride);

		RCache.set_Shader	(s_postprocess_ssr_temporal);
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
		RCache.set_c("dx_matrix_PreviousProjection", m_previous);
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

		Fvector4 SSRParams = {ps_r2_ssr_temporal_response, ps_r2_ssr_atten_border, 0, 0};
		RCache.set_c("dx_SSR_Params", SSRParams);

		RCache.set_Geometry			(g_postprocess_ssr_temporal);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}

	if(1)
	{
		u32			Offset					= 0;
		Fvector2	p0,p1;

		u_setrt	(rt_ssr_reflection_previous, 0,0,0, HW.pBaseZB );
		RCache.set_CullMode(CULL_NONE);
		RCache.set_Stencil(FALSE);

		struct v_blit_ssr	
		{
			Fvector4	p;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_blit_ssr* pv = (v_blit_ssr*)
			
		RCache.Vertex.Lock(4, g_blit_ssr->vb_stride, Offset);

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

		RCache.Vertex.Unlock(4, g_blit->vb_stride);

		RCache.set_Shader(s_blit_ssr);
		RCache.set_Geometry	(g_blit_ssr);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}
}