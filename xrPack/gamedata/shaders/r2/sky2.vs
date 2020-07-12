#include "common.h"

struct vi
{
        float4        p        : POSITION        	;
		float3        normal        : NORMAL        	;
        float4        c        : COLOR0        		;
        float3        tc0      : TEXCOORD0        	;
        float3        tc1      : TEXCOORD1        	;
};

struct vf
{
        float4        hpos     : POSITION        	;
		float4			pos 	: TEXCOORD2;
		float3			normal 	: TEXCOORD4;
        float4        factor        : COLOR0        		;
        float3        tc0      : TEXCOORD0        	;
        float3        tc1      : TEXCOORD1        	;
		float4		screenPos: TEXCOORD3;
};

vf main (vi v)
{
        vf                 	o;
		//o.hpos = mul(m_WVP, v.p);
        o.hpos = mul(m_WVP, float4(v.p.xyz , 1.0f)).xyww;
		o.pos = v.p;
		o.normal = v.normal;
        o.tc0 = v.tc0;
        o.tc1 = v.tc1;  

        // Deprecated
        /*#ifdef USE_VTF
                float	scale		= tex2Dlod	(s_tonemap,float4(.5,.5,.5,.5)).x ;
                o.factor                	= float4	( v.c.rgb*scale*2.0, v.c.a );      		// copy color, pre-scale by tonemap //float4 ( v.c.rgb*scale*2, v.c.a );
        #else
                o.factor                	= v.c       ;										// copy color, low precision
        #endif*/
		o.factor = v.c;	
		o.screenPos = ComputeScreenPos(o.hpos);
        return              o;
}
FXVS;