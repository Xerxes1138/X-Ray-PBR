#include "stdafx.h"
#pragma hdrstop

#include "blender_postprocess_bloom_prepass.h"

CBlender_postprocess_bloom_prepass::CBlender_postprocess_bloom_prepass	()	{	description.CLS		= 0;	}
CBlender_postprocess_bloom_prepass::~CBlender_postprocess_bloom_prepass	()	{	}
 
void	CBlender_postprocess_bloom_prepass::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_bloom_prepass",	FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_currentColor",	r2_RT_currentColor);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}
