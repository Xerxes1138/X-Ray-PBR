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
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}

CBlender_postprocess_ssgi::CBlender_postprocess_ssgi	()	{	description.CLS		= 0;	}
CBlender_postprocess_ssgi::~CBlender_postprocess_ssgi	()	{	}
 
void	CBlender_postprocess_ssgi::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_ssgi",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}
