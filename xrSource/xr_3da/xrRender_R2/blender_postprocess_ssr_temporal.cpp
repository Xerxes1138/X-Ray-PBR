#include "stdafx.h"
#pragma hdrstop

#include "blender_postprocess_ssr_temporal.h"

CBlender_postprocess_ssr_temporal::CBlender_postprocess_ssr_temporal	()	{	description.CLS		= 0;	}
CBlender_postprocess_ssr_temporal::~CBlender_postprocess_ssr_temporal	()	{	}
 
void	CBlender_postprocess_ssr_temporal::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_ssr_temporal",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",					r2_RT_specular);

	C.r_Sampler_rtf		("s_motionVector",	r2_RT_motionVector);
	
	C.r_Sampler_rtf		("s_SSR_Raycast",	r2_RT_SSR_Raycast);

	C.r_Sampler_rtf		("s_currentColor",	/*r2_RT_generic1*/ r2_RT_SSR_Resolve);

	C.r_Sampler_clf		("s_previousColor",	r2_RT_SSR_Temporal);

	C.r_Sampler			("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}

CBlender_postprocess_ssgi_temporal::CBlender_postprocess_ssgi_temporal	()	{	description.CLS		= 0;	}
CBlender_postprocess_ssgi_temporal::~CBlender_postprocess_ssgi_temporal	()	{	}
 
void	CBlender_postprocess_ssgi_temporal::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_ssgi_temporal",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler_rtf		("s_motionVector",	r2_RT_motionVector);
	
	C.r_Sampler_rtf		("s_SSGI_Raycast",	r2_RT_SSGI_Raycast);

	C.r_Sampler_rtf		("s_currentColor",	r2_RT_SSGI_Resolve);

	C.r_Sampler_clf		("s_previousColor",	r2_RT_SSGI_Previous);

	C.r_Sampler			("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}