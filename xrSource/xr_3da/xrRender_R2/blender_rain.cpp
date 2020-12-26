#include "stdafx.h"
#include "blender_rain.h"


CBlender_rain::CBlender_rain() {description.CLS	= 0;}
CBlender_rain::~CBlender_rain() {}

void CBlender_rain::Compile(CBlender_Compile& C)
{
	IBlender::Compile	(C);

	C.r_Pass	("null", "deferred_rain", FALSE,	FALSE,	FALSE);

	C.r_Sampler_rtf		("s_position",		r2_RT_P);
	C.r_Sampler_rtf		("s_normal",		r2_RT_N);
	C.r_Sampler_rtf		("s_diffuse",		r2_RT_albedo);
	C.r_Sampler_rtf		("s_specular",		r2_RT_specular);

	C.r_Sampler_rtf		("s_material",		r2_material);
	C.r_Sampler_rtf		("s_accumulator",	r2_RT_accum);
	C.r_Sampler_rtf		("s_lmap",			r2_sunmask);
	C.r_Sampler_rtf		("s_smap",			r2_RT_smap_depth);

	jitter				(C);
	C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0", false, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT, FALSE);
	//C.r_Sampler		("s_bluenoise", "engine\\LDR_RGBA_0");
	C.r_Sampler		("s_Rain_Puddle", "weather\\rain_puddle_01");
	C.r_Sampler		("s_Rain_Ripple", "weather\\rain_ripple_01");

	C.r_Sampler_rtf		("s_water",	"water\\water_normal");

	C.r_End				();
}



