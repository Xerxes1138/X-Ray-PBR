#include "stdafx.h"
#pragma hdrstop

#include "Blender_blit.h"

CBlender_blit::CBlender_blit() {description.CLS	= 0;}
CBlender_blit::~CBlender_blit() {}
 
void CBlender_blit::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "blit_taa", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_currentColor);
	C.r_Sampler_clf("s_previousColor", r2_RT_previousColor);
	C.r_Sampler_clf("s_motionVector",	r2_RT_motionVector);

	C.r_End();
}

CBlender_blit_ssr::CBlender_blit_ssr() {description.CLS	= 0;}
CBlender_blit_ssr::~CBlender_blit_ssr() {}
 
void CBlender_blit_ssr::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "blit", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_SSR_Reflection);
	C.r_Sampler_clf("s_motionVector",	r2_RT_motionVector);

	C.r_End();
}
