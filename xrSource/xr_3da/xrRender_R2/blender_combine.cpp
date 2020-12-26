#include "stdafx.h"
#pragma hdrstop

#include "Blender_combine.h"

CBlender_combine::CBlender_combine	()	{	description.CLS		= 0;	}
CBlender_combine::~CBlender_combine	()	{	}
 
void	CBlender_combine::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	switch (C.iElement)
	{
	case 0:	// combine
		//C.r_Pass			("combine_1",		"combine_1",		FALSE,	FALSE,	FALSE, TRUE, D3DBLEND_INVSRCALPHA, D3DBLEND_SRCALPHA);	//. MRT-blend?
		C.r_Pass			("null",		"deferred_resolve",		FALSE,	FALSE,	FALSE);
		C.r_Sampler_rtf		("s_position",		r2_RT_P);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N);
		C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);

		C.r_Sampler_rtf		("s_accumulator",	r2_RT_accum);

		//C.r_Sampler_rtf		("s_accumulator",	r2_RT_accum_ssgi);

		C.r_Sampler_rtf		("s_accumulatorSpecular",	r2_RT_accumSpecular);

		C.r_Sampler_rtf		("s_specular",					r2_RT_specular);
		C.r_Sampler_rtf		("s_tonemap",				r2_RT_luminance_cur);
		C.r_Sampler_clw		("s_material",				r2_material);

		C.r_Sampler			("s_diffuse_s0",			r2_T_envs0);
		C.r_Sampler			("s_diffuse_s1",			r2_T_envs1);
		C.r_Sampler			("s_specular_s0",			r2_T_sky0);
		C.r_Sampler			("s_specular_s1",			r2_T_sky1);

		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_End				();
		break;
	case 1:	// aa-edge-detection + AA :)
		C.r_Pass			("null",			"combine_2_AA",		FALSE,	FALSE,	FALSE);
		C.r_Sampler_rtf		("s_position",		r2_RT_P);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N);
		C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
		C.r_Sampler_clf		("s_image",			r2_RT_generic0);
		C.r_Sampler_clf		("s_bloom",			r2_RT_bloom1);
		C.r_Sampler_clf		("s_distort",		r2_RT_generic1);
		C.r_Sampler_rtf		("s_specular",					r2_RT_specular);

		C.r_Sampler_rtf		("s_accumulatorSpecular",	r2_RT_accumSpecular);
				C.r_Sampler_rtf		("s_currentColor",	r2_RT_currentColor);
		C.r_Sampler_rtf		("s_previousColor",	r2_RT_previousColor);

		C.r_Sampler			("env_s0",			r2_T_envs0);
		C.r_Sampler			("env_s1",			r2_T_envs1);
		C.r_Sampler			("sky_s0",			r2_T_sky0);
		C.r_Sampler			("sky_s1",			r2_T_sky1);

		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_End				();
		break;
	case 2:	// non-AA
		C.r_Pass			("null",			"combine_2_NAA",	FALSE,	FALSE,	FALSE);
		C.r_Sampler_rtf		("s_position",		r2_RT_P);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N);
		C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
		C.r_Sampler_clf		("s_image",			r2_RT_generic0);
		C.r_Sampler_clf		("s_bloom",			r2_RT_bloom1);
		C.r_Sampler_clf		("s_distort",		r2_RT_generic1);
		C.r_Sampler_rtf		("s_specular",					r2_RT_specular);

		C.r_Sampler_rtf		("s_accumulatorSpecular",	r2_RT_accumSpecular);
				C.r_Sampler_rtf		("s_currentColor",	r2_RT_currentColor);
				C.r_Sampler_rtf		("s_previousColor",	r2_RT_previousColor);

		C.r_Sampler			("env_s0",			r2_T_envs0);
		C.r_Sampler			("env_s1",			r2_T_envs1);
		C.r_Sampler			("sky_s0",			r2_T_sky0);
		C.r_Sampler			("sky_s1",			r2_T_sky1);

		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_End				();
		break;
	case 3:	// aa-edge-detection + AA :) + DISTORTION
		C.r_Pass			("null",			"combine_2_AA_D",	FALSE,	FALSE,	FALSE);
		C.r_Sampler_rtf		("s_position",		r2_RT_P);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N);
		C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
		C.r_Sampler_clf		("s_image",			r2_RT_generic0);
		C.r_Sampler_clf		("s_bloom",			r2_RT_bloom1);
		C.r_Sampler_clf		("s_distort",		r2_RT_generic1);
		C.r_Sampler_rtf		("s_specular",					r2_RT_specular);

		C.r_Sampler_rtf		("s_accumulatorSpecular",	r2_RT_accumSpecular);
				C.r_Sampler_rtf		("s_currentColor",	r2_RT_currentColor);
				C.r_Sampler_rtf		("s_previousColor",	r2_RT_previousColor);

		C.r_Sampler			("env_s0",			r2_T_envs0);
		C.r_Sampler			("env_s1",			r2_T_envs1);
		C.r_Sampler			("sky_s0",			r2_T_sky0);
		C.r_Sampler			("sky_s1",			r2_T_sky1);

		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_End				();
		break;
	case 4:	// non-AA + DISTORTION
		C.r_Pass			("null",			"combine_2_NAA_D",	FALSE,	FALSE,	FALSE);
		C.r_Sampler_rtf		("s_position",		r2_RT_P);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N);
		C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
		C.r_Sampler_clf		("s_image",			r2_RT_generic0);
		C.r_Sampler_clf		("s_bloom",			r2_RT_bloom1);
		C.r_Sampler_clf		("s_distort",		r2_RT_generic1);
		C.r_Sampler_rtf		("s_specular",					r2_RT_specular);

		C.r_Sampler_rtf		("s_accumulatorSpecular",	r2_RT_accumSpecular);

						C.r_Sampler_rtf		("s_currentColor",	r2_RT_currentColor);

				C.r_Sampler_rtf		("s_previousColor",	r2_RT_previousColor);

		C.r_Sampler			("env_s0",			r2_T_envs0);
		C.r_Sampler			("env_s1",			r2_T_envs1);
		C.r_Sampler			("sky_s0",			r2_T_sky0);
		C.r_Sampler			("sky_s1",			r2_T_sky1);

		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_End				();
		break;
	}
}
