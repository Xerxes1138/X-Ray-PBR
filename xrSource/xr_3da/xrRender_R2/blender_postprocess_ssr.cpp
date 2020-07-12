#include "stdafx.h"
#pragma hdrstop

#include "blender_postprocess_ssr.h"

CBlender_postprocess_ssr::CBlender_postprocess_ssr	()	{	description.CLS		= 0;	}
CBlender_postprocess_ssr::~CBlender_postprocess_ssr	()	{	}
 
void	CBlender_postprocess_ssr::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_ssr",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_depth",			r2_RT_depth);

	/*C.r_Sampler_rtf		("s_currentColor",	r2_RT_currentColor);

	C.r_Sampler			("s_diffuse_s0",			r2_T_envs0);
	C.r_Sampler			("s_diffuse_s1",			r2_T_envs1);

	C.r_Sampler			("s_specular_s0",			r2_T_sky0);
	C.r_Sampler			("s_specular_s1",			r2_T_sky1);*/

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}
