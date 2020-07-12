#include "common.h"
#include "shared\waterconfig.h"
#include "shared\watermove.h"

struct	v_vert
{
	float4 	P	: POSITION;	// (float,float,float,1)
	float4	N	: NORMAL;	// (nx,ny,nz,hemi occlusion)
	float4 	T	: TANGENT;
	float4 	B	: BINORMAL;
	float4	color	: COLOR0;	// (r,g,b,dir-occlusion)
	float2 	uv	: TEXCOORD0;	// (u0,v0)
};
struct vf
{
	float4 hpos	: POSITION	;
	float2 tbase	: TEXCOORD0	;
	float2 tdist0	: TEXCOORD1	;
	float2 tdist1	: TEXCOORD2	;
};

vf main (v_vert v)
{
	vf 		o;

	float4 	P 	= v.P;
	float3 	N 	= unpack_normal		(v.N);
		//P 	= watermove		(P);

	o.tbase		= unpack_tc_base	(v.uv,v.T.w,v.B.w);		// copy tc
	
    o.tdist0        = ( P.xz / 4) + timers.x * float2(0.2, 0.0);
	o.tdist1        = ( P.xz / 4) + timers.x * float2(-0.2, 0.0) + float2(0.25, -0.25);
	
	o.hpos 		= mul			(m_VP, P);			// xform, input in world coords

	return o;
}
