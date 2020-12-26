#include "stdafx.h"
#pragma hdrstop

#include "Blender_light_direct.h"

CBlender_accum_direct::CBlender_accum_direct	()	{	description.CLS		= 0;	}
CBlender_accum_direct::~CBlender_accum_direct	()	{	}

void	CBlender_accum_direct::Compile(CBlender_Compile& C)
{
	IBlender::Compile		(C);

	BOOL	b_HW_smap		= RImplementation.o.HW_smap;
	BOOL	b_HW_PCF		= RImplementation.o.HW_smap_PCF;
	BOOL		blend		= FALSE;	//RImplementation.o.fp16_blend;
	D3DBLEND	dest		= blend?D3DBLEND_ONE:D3DBLEND_ZERO;
	if (RImplementation.o.sunfilter)	{ blend = FALSE; dest = D3DBLEND_ZERO; }

	switch (C.iElement)
	{
	case SE_SUN_NEAR:		// near pass - enable Z-test to perform depth-clipping
		C.r_Pass			("null",			"accum_sun_near",	false,	TRUE,	FALSE,blend,D3DBLEND_ONE,dest);
		C.PassSET_ZB		(TRUE,FALSE,TRUE	);	// force inverted Z-Buffer
		C.r_Sampler_rtf		("s_position",		r2_RT_P			);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N			);
		C.r_Sampler_rtf		("s_specular",		r2_RT_specular	);
		C.r_Sampler_clw		("s_material",		r2_material		);
		C.r_Sampler_rtf		("s_accumulator",	r2_RT_accum		);

		C.r_Sampler_rtf		("s_AO",			r2_RT_AO		);

		C.r_Sampler			("s_lmap",			r2_sunmask		);
		if (b_HW_smap)		{
			if (b_HW_PCF)	C.r_Sampler_clf		("s_smap",r2_RT_smap_depth	);
			else			{
				C.r_Sampler_rtf		("s_smap",r2_RT_smap_depth	);
			}
		}
		else				C.r_Sampler_rtf		("s_smap",r2_RT_smap_surf	);
		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_Sampler		("s_sun_lut", "engine\\Sun_Lut_LDR", false, D3DTADDRESS_CLAMP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		jitter				(C);
		C.r_End				();
		break;
	case SE_SUN_FAR:		// far pass, only stencil clipping performed
		C.r_Pass			("null",			"accum_sun_far",	false,	TRUE,	FALSE,blend,D3DBLEND_ONE,dest);
		C.r_Sampler_rtf		("s_position",		r2_RT_P			);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N			);
		C.r_Sampler_rtf		("s_specular",					r2_RT_specular);
		C.r_Sampler_clw		("s_material",		r2_material		);
		C.r_Sampler_rtf		("s_accumulator",	r2_RT_accum		);
		C.r_Sampler			("s_lmap",			r2_sunmask		);

		C.r_Sampler_rtf		("s_AO",			r2_RT_AO		);

		if (b_HW_smap)		{
			if (b_HW_PCF)	C.r_Sampler_clf		("s_smap",r2_RT_smap_depth	);
			else			C.r_Sampler_rtf		("s_smap",r2_RT_smap_depth	);
		}
		else				C.r_Sampler_rtf		("s_smap",r2_RT_smap_surf	);
		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_Sampler		("s_sun_lut", "engine\\Sun_Lut_LDR", false, D3DTADDRESS_CLAMP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);
		jitter				(C);
		C.r_End				();
		break;
	case SE_SUN_LUMINANCE:	// luminance pass
		C.r_Pass			("null",			"accum_sun",		false,	FALSE,	FALSE);
		C.r_Sampler_rtf		("s_position",		r2_RT_P			);
		C.r_Sampler_rtf		("s_normal",		r2_RT_N			);
		C.r_Sampler_rtf		("s_specular",					r2_RT_specular);
		C.r_Sampler_clw		("s_material",		r2_material		);
		C.r_Sampler_clf		("s_smap",			r2_RT_generic0	);
		C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);

		C.r_Sampler		("s_sun_lut", "engine\\Sun_Lut_LDR", false, D3DTADDRESS_CLAMP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);
		jitter				(C);
		C.r_End				();
		break;
	}
}
