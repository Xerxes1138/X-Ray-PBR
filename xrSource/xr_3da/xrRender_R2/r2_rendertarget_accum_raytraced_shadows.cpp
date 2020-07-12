#include "stdafx.h"

#include "..\igame_persistent.h"
#include "..\environment.h"

void CRenderTarget::accum_raytraced_shadows ()
{
	{	
		CEnvDescriptorMixer& environment = g_pGamePersistent->Environment().CurrentEnv;

		bool	_menu_pp	= g_pGamePersistent?g_pGamePersistent->OnRenderPPUI_query():false;

		u32			Offset					= 0;
		Fvector2	p0, p1;

		u_setrt						(rt_Accumulator, rt_AccumulatorSpecular, NULL, NULL, HW.pBaseZB);

		struct v_accum_raytraced_shadows
		{
			Fvector4	position;
			Fvector2	uv0;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);

		p0.set(.5f/_w, .5f/_h);
		p1.set((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_accum_raytraced_shadows* pv = (v_accum_raytraced_shadows*) RCache.Vertex.Lock(4, g_accum_raytraced_shadows->vb_stride, Offset);
		pv->position.set(EPS, float(_h+EPS), EPS, 1.f);
		pv->uv0.set(p0.x, p1.y);
		pv++;

		pv->position.set(EPS, EPS,	EPS, 1.f);
		pv->uv0.set(p0.x, p0.y);
		pv++;

		pv->position.set(float(_w+EPS), float(_h+EPS), EPS, 1.f); 
		pv->uv0.set(p1.x, p1.y);
		pv++;

		pv->position.set(float(_w+EPS), EPS, EPS, 1.f); 
		pv->uv0.set(p1.x, p0.y); 
		pv++;

		RCache.Vertex.Unlock(4, g_accum_raytraced_shadows->vb_stride);

		// Shader
		RCache.set_Shader(s_accum_raytraced_shadows);

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

		Fvector4 SSRSarams = {ps_r2_contact_shadows_sun_length, ps_r2_contact_shadows_length, 0, 0};
		RCache.set_c("dx_SSRS_Params", SSRSarams);

		RCache.set_Geometry	(g_accum_raytraced_shadows);
		RCache.Render(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}
}