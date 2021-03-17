#include "stdafx.h"
#pragma hdrstop

#include "Blender_ambient.h"

CBlender_ambient::CBlender_ambient	()	{	description.CLS		= 0;	}
CBlender_ambient::~CBlender_ambient	()	{	}
 
void	CBlender_ambient::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	C.r_Pass			("null",		"accum_ambient", FALSE, FALSE, FALSE, TRUE, D3DBLEND_ONE, D3DBLEND_ONE);

	C.r_Sampler_rtf		("s_position",				r2_RT_P);
	C.r_Sampler_rtf		("s_normal",				r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",				r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",				r2_RT_specular);

	C.r_Sampler_rtf		("s_SSGI_Resolve",			r2_RT_SSGI_Previous);

	C.r_Sampler_rtf		("s_reflectionColor",		r2_RT_SSR_Temporal);

	C.r_Sampler_rtf		("s_motionVector",			r2_RT_motionVector);
	C.r_Sampler_rtf		("s_AO",					r2_RT_AO_Temporal);

	C.r_Sampler			("s_diffuse_s0",			r2_T_envs0, D3DTADDRESS_CLAMP,	D3DTEXF_LINEAR,		D3DTEXF_LINEAR,	D3DTEXF_LINEAR, true		);
	C.r_Sampler			("s_diffuse_s1",			r2_T_envs1, D3DTADDRESS_CLAMP,	D3DTEXF_LINEAR,		D3DTEXF_LINEAR,	D3DTEXF_LINEAR, true		);
	C.r_Sampler			("s_specular_s0",			r2_T_sky0, D3DTADDRESS_CLAMP,	D3DTEXF_LINEAR,		D3DTEXF_LINEAR,	D3DTEXF_LINEAR, true		);
	C.r_Sampler			("s_specular_s1",			r2_T_sky1, D3DTADDRESS_CLAMP,	D3DTEXF_LINEAR,		D3DTEXF_LINEAR,	D3DTEXF_LINEAR, true		);

	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

	C.r_End				();
}