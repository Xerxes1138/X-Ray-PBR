#include "stdafx.h"
#pragma hdrstop

#include "blender_motionVector.h"

CBlender_motionVector::CBlender_motionVector	()	{	description.CLS		= 0;	}
CBlender_motionVector::~CBlender_motionVector	()	{	}
 
void	CBlender_motionVector::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",			"motionVector",	FALSE,	TRUE,	FALSE);

	C.r_Sampler_rtf		("s_position",				r2_RT_P);
	C.r_Sampler_rtf		("s_normal",				r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",				r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",					r2_RT_specular);


	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}