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
	C.r_Sampler_rtf		("s_depth",			r2_RT_depth);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}
