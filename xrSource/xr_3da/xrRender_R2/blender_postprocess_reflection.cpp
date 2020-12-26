#include "stdafx.h"
#pragma hdrstop

#include "blender_postprocess_reflection.h"

CBlender_postprocess_reflection::CBlender_postprocess_reflection	()	{	description.CLS		= 0;	}
CBlender_postprocess_reflection::~CBlender_postprocess_reflection	()	{	}
 
void	CBlender_postprocess_reflection::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	
	BOOL raycastHalf = ps_r2_ssr_flags.test(R2FLAG_SSR_RAYCAST_HALF);

	if(raycastHalf)
	{
		C.r_Pass			("null",			"postprocess_ssr_resolve_raycast_half",	FALSE,	FALSE,	FALSE);
	}
	else
	{
		C.r_Pass			("null",			"postprocess_ssr_resolve",	FALSE,	FALSE,	FALSE);
	}

	C.r_Sampler_rtf		("s_position",				r2_RT_P);
	C.r_Sampler_rtf		("s_normal",				r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",				r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",					r2_RT_specular);

	C.r_Sampler_rtf		("s_SSR_Raycast",			r2_RT_SSR_Raycast);

	C.r_Sampler_rtf		("s_currentColor",			r2_RT_SSGI_Combine);

	C.r_Sampler			("s_diffuse_s0",			r2_T_envs0);
	C.r_Sampler			("s_diffuse_s1",			r2_T_envs1);

	C.r_Sampler			("s_specular_s0",			r2_T_sky0);
	C.r_Sampler			("s_specular_s1",			r2_T_sky1);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}

CBlender_postprocess_ssgi_resolve::CBlender_postprocess_ssgi_resolve	()	{	description.CLS		= 0;	}
CBlender_postprocess_ssgi_resolve::~CBlender_postprocess_ssgi_resolve	()	{	}
 
void	CBlender_postprocess_ssgi_resolve::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);
	
	C.r_Pass			("null",			"postprocess_ssgi_resolve",	FALSE,	FALSE,	FALSE);
	
	C.r_Sampler_rtf		("s_position",				r2_RT_P);
	C.r_Sampler_rtf		("s_normal",				r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",				r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",				r2_RT_specular);

	C.r_Sampler_rtf		("s_SSGI_Raycast",			r2_RT_SSGI_Raycast);

	C.r_Sampler_rtf		("s_currentColor",			r2_RT_SSGI_Combine);

	C.r_Sampler			("s_diffuse_s0",			r2_T_envs0);
	C.r_Sampler			("s_diffuse_s1",			r2_T_envs1);

	C.r_Sampler			("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}