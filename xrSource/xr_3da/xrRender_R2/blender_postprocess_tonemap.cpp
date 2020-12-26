#include "stdafx.h"
#pragma hdrstop

#include "blender_postprocess_tonemap.h"

CBlender_postprocess_tonemap_exposure::CBlender_postprocess_tonemap_exposure	()	{	description.CLS		= 0;	}
CBlender_postprocess_tonemap_exposure::~CBlender_postprocess_tonemap_exposure	()	{	}
 
void	CBlender_postprocess_tonemap_exposure::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",		"postprocess_tonemap_exposure", FALSE, FALSE, FALSE);

	if(1)
	{
		C.r_Sampler_clf			("s_currentColor",	r2_RT_previousColor);
	}
	else
	{
		C.r_Sampler_clf			("s_currentColor",	r2_RT_generic1);
	}


	C.r_Sampler_rtf		("s_tonemap",				r2_RT_luminance_cur);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}

CBlender_postprocess_tonemap::CBlender_postprocess_tonemap	()	{	description.CLS		= 0;	}
CBlender_postprocess_tonemap::~CBlender_postprocess_tonemap	()	{	}
 
void	CBlender_postprocess_tonemap::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",		"postprocess_tonemap", FALSE, FALSE, FALSE);
	//C.r_Pass			("null",		"postprocess_tonemap", FALSE, FALSE, FALSE,FALSE, D3DBLEND_ONE, D3DBLEND_ZERO, FALSE, 0, FALSE);

	C.r_Sampler_clf		("s_currentColor",	/*r2_RT_previousColor*/ r2_RT_generic0);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler_rtf		("s_AO",			r2_RT_AO);
	C.r_Sampler_rtf		("s_SSGI_Raycast",	r2_RT_SSGI_Raycast);
	C.r_Sampler_rtf		("s_SSR_Raycast",	r2_RT_SSR_Raycast);
	C.r_Sampler_clf		("s_motionVector",	r2_RT_motionVector);

	C.r_Sampler_rtf		("s_SSGI_Resolve",			r2_RT_SSGI_Previous);
	C.r_Sampler_rtf		("s_reflectionColor",		r2_RT_SSR_Reflection_Previous);

	C.r_Sampler_clf		("s_distort",		r2_RT_distortion);

	C.r_Sampler_clf		("s_bloom",			r2_RT_bloom1);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}