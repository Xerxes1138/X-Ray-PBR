#include "stdafx.h"
#pragma hdrstop

#include "blender_postprocess_AO.h"
 
CBlender_postprocess_AO::CBlender_postprocess_AO	()	{	description.CLS		= 0;	}
CBlender_postprocess_AO::~CBlender_postprocess_AO	()	{	}

void	CBlender_postprocess_AO::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_AO",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler_rtf		("s_smap",			r2_RT_smap_depth);

	C.r_Sampler			("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}

CBlender_postprocess_AO_resolve::CBlender_postprocess_AO_resolve	()	{	description.CLS		= 0;	}
CBlender_postprocess_AO_resolve::~CBlender_postprocess_AO_resolve	()	{	}

void	CBlender_postprocess_AO_resolve::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_ssao_Resolve",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler_rtf		("s_AO",			r2_RT_AO);

	C.r_Sampler_rtf		("s_smap",			r2_RT_smap_depth);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}


CBlender_postprocess_AO_temporal::CBlender_postprocess_AO_temporal	()	{	description.CLS		= 0;	}
CBlender_postprocess_AO_temporal::~CBlender_postprocess_AO_temporal	()	{	}

void	CBlender_postprocess_AO_temporal::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_ssao_Temporal",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler_rtf		("s_motionVector",	r2_RT_motionVector);

	C.r_Sampler_rtf		("s_currentColor",	r2_RT_AO_Resolve);
	C.r_Sampler_clf		("s_previousColor",	r2_RT_AO_Temporal);

	C.r_Sampler_rtf		("s_smap",			r2_RT_smap_depth);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}
