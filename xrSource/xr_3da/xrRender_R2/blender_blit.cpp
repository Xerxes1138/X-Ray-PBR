#include "stdafx.h"
#pragma hdrstop

#include "Blender_blit.h"

CBlender_blit::CBlender_blit() {description.CLS	= 0;}
CBlender_blit::~CBlender_blit() {}
 
void CBlender_blit::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "blit", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_generic1);

	C.r_End();
}

CBlender_blit_taa::CBlender_blit_taa() {description.CLS	= 0;}
CBlender_blit_taa::~CBlender_blit_taa() {}
 
void CBlender_blit_taa::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "blit_taa", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_currentColor);
	C.r_Sampler_clf("s_previousColor", r2_RT_previousColor);
	C.r_Sampler_clf("s_motionVector",	r2_RT_motionVector);

	C.r_End();
}

CBlender_blit_ssgi_recursive::CBlender_blit_ssgi_recursive() {description.CLS	= 0;}
CBlender_blit_ssgi_recursive::~CBlender_blit_ssgi_recursive() {}
 
void CBlender_blit_ssgi_recursive::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "blit_ssgi_recursive", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_previousColor /*r2_RT_generic0*/);
	C.r_Sampler_clf("s_motionVector", r2_RT_motionVector);

	C.r_End();
}

CBlender_blit_ssao::CBlender_blit_ssao() {description.CLS	= 0;}
CBlender_blit_ssao::~CBlender_blit_ssao() {}
 
void CBlender_blit_ssao::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "blit", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_AO_Resolve);
	C.r_Sampler_clf("s_motionVector", r2_RT_motionVector);

	C.r_End();
}

CBlender_blit_ssgi::CBlender_blit_ssgi() {description.CLS	= 0;}
CBlender_blit_ssgi::~CBlender_blit_ssgi() {}
 
void CBlender_blit_ssgi::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "blit", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_SSGI);
	C.r_Sampler_clf("s_motionVector", r2_RT_motionVector);

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

CBlender_postprocess_fog_scattering::CBlender_postprocess_fog_scattering() {description.CLS	= 0;}
CBlender_postprocess_fog_scattering::~CBlender_postprocess_fog_scattering() {}
 
void CBlender_postprocess_fog_scattering::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass("null", "postprocess_fog_scattering", FALSE,	FALSE, FALSE);

	C.r_Sampler_clf("s_currentColor", r2_RT_generic1);
	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End();
}
