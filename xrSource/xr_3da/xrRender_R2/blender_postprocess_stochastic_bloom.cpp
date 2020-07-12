#include "stdafx.h"
#pragma hdrstop

#include "blender_postprocess_stochastic_bloom.h"

CBlender_postprocess_stochastic_bloom::CBlender_postprocess_stochastic_bloom	()	{	description.CLS		= 0;	}
CBlender_postprocess_stochastic_bloom::~CBlender_postprocess_stochastic_bloom	()	{	}
 
void	CBlender_postprocess_stochastic_bloom::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"postprocess_bloom",	FALSE,	FALSE,	FALSE);
	C.r_Sampler_rtf		("s_currentColor",	r2_RT_currentColor);
	C.r_Sampler_rtf		("s_bloom",			r2_RT_bloom1);
	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);
	C.r_End				();
}
