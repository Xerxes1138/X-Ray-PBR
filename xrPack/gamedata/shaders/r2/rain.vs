#include "common.h"

struct        v_vert
{
        float4 P        : POSITION;
        float4 N        : NORMAL;   
        float4 T        : TANGENT;
        float4 B        : BINORMAL;
        float4 color        : COLOR0; 
        float2 uv : TEXCOORD0;
};

struct   vf
{
        float4         hpos        :         POSITION        ;
        float2  tbase        :        TEXCOORD0        ;
        float3         M1                :        TEXCOORD3        ;
        float3         M2                :        TEXCOORD4        ;
        float3         M3                :        TEXCOORD5        ;
        float4        c0                :          COLOR0                ;
        float          fog        :         FOG                ;
		float4          screenPos        :         TEXCOORD7                ;
		float3          positionWorld       :         TEXCOORD6               ;
};

vf main(v_vert v)
{
        vf                 o;

        float4 P = v.P;

        o.tbase = v.uv; 
		
		float3 positionWorld = mul(m_W, half4(P.xyz, 1.0f)); 
		
		o.positionWorld = positionWorld;

		float3 normal = normalize(mul((float3x3)m_W, unpack_bx2(v.N.xyz)));
		float3 tangent = normalize(mul((float3x3)m_W, unpack_bx2(v.T.xyz)));
		float sign = v.T.w;
		float3 binormal = normalize(cross(normal, tangent) * sign);

        o.M1                 = tangent;
        o.M2                 = binormal;
        o.M3                 = normal;

        o.hpos = mul(m_WVP, P);
		
		float3 positionView = mul(m_WV, P);
		
		float fogHeight = ComputeHeightFog(positionWorld, eye_position, dx_WeatherParams.x);
	
		#if 0
			float fog = saturate(ComputeLegacyFog(positionView) * fogHeight);
		#else
			float fog = saturate(ComputeExponentialFog(positionView) * fogHeight);
		#endif
		
		
		o.fog       = fog;

		o.c0		= v.color;
		
		o.screenPos = ComputeScreenPos(o.hpos);
		o.screenPos.z = mul(m_V, P).z;
			
        return o;
}
