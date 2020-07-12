#include "common.h"

uniform float4 		consts; // {1/quant,1/quant,diffusescale,ambient}
uniform float4 		array	[200] : register(c12);

v_shadow_direct_aref 	main (v_detail v)
{
	v_shadow_direct_aref 		O;
	// index
	int 	i 	= v.misc.w;
	float4  m0 	= array[i+0];
	float4  m1 	= array[i+1];
	float4  m2 	= array[i+2];
	float4  c0 	= array[i+3];

	// Transform pos to world coords
	float4 	pos;
 	pos.x 		= dot	(m0, v.pos);
 	pos.y 		= dot	(m1, v.pos);
 	pos.z 		= dot	(m2, v.pos);
	pos.w 		= 1;

	O.hpos 		= mul(m_WVP, pos);

	O.tc0 		= v.misc.xy * consts.xy;

	#ifndef USE_HWSMAP
		O.depth = O.hpos.z;
	#endif

	return O;
}
FXVS;
