#include "common.h"
#include "shared\waterconfig.h"
#include "shared\watermove.h"

struct        v_vert
{
        float4         P        : POSITION;        // (float,float,float,1)
        float4        N        : NORMAL;        // (nx,ny,nz,hemi occlusion)
        float4         T        : TANGENT;
        float4         B        : BINORMAL;
        float4        color        : COLOR0;        // (r,g,b,dir-occlusion)
        float2         uv        : TEXCOORD0;        // (u0,v0)
};

struct   vf
{
        float4      hpos        :         POSITION;
        float2  	tbase        :           TEXCOORD0;  // base
        half3       M1        :        TEXCOORD3        ;
        half3       M2        :        TEXCOORD4        ;
        half3       M3        	:        TEXCOORD5        ;
        //half3       v2point     :        TEXCOORD6        ;
        half4        c0        :          COLOR0                ;
        float          fog        :         FOG                ;
		float4          screenPos        :         TEXCOORD7                ;
		float3          positionWorld        :         TEXCOORD6                ;
};

vf main (v_vert v)
{
        vf                 o;

        float4         P         = v.P        ;                // world
        float3         NN         = unpack_normal        (v.N)        ;
                P         = watermove        (P)        ;

        //o.v2point        = /*mul(m_WV, P);*/P-eye_position        ;
        o.tbase = unpack_tc_base        (v.uv,v.T.w,v.B.w);                // copy tc
		
		float3 positionWorld = mul(m_W, half4(P.xyz, 1.0f)); 
		
		o.positionWorld = positionWorld;
		
		
        // Calculate the 3x3 transform from tangent space to eye-space
        // TangentToEyeSpace = object2eye * tangent2object
        //                     = object2eye * transpose(object2tangent) (since the inverse of a rotation is its transpose)
        float3          N         = unpack_bx2(v.N);        // just scale (assume normal in the -.5f, .5f)
        float3          T         = unpack_bx2(v.T);        //
        float3          B         = unpack_bx2(v.B);        //
        float3x3 xform        = mul        ((float3x3)m_W, float3x3(
                                                T.x,B.x,N.x,
                                                T.y,B.y,N.y,
                                                T.z,B.z,N.z
                                ));
        // The pixel shader operates on the bump-map in [0..1] range
        // Remap this range in the matrix, anyway we are pixel-shader limited :)
        // ...... [ 2  0  0  0]
        // ...... [ 0  2  0  0]
        // ...... [ 0  0  2  0]
        // ...... [-1 -1 -1  1]
        // issue: strange, but it's slower :(
        // issue: interpolators? dp4? VS limited? black magic?

		float3 normal = normalize(mul((float3x3)m_W, unpack_bx2(v.N.xyz)));
		float3 tangent = normalize(mul((float3x3)m_W, unpack_bx2(v.T.xyz)));
		float sign = v.T.w;
		float3 binormal = normalize(cross(normal, tangent) * sign);

        // Feed this transform to pixel shader
        o.M1                 = tangent; //xform        [0];
        o.M2                 = binormal; //xform        [1];
        o.M3                 = normal; //xform        [2];

        float3         L_rgb         = v.color.xyz;                                                // precalculated RGB lighting
        float3         L_hemi         = v_hemi(N)*v.N.w;                                        // hemisphere
        float3         L_sun         = v_sun(N)*v.color.w;                                        // sun
        float3         L_final        = L_rgb + L_hemi + L_sun + L_ambient;
                // L_final        = v.N.w        + L_ambient;

        o.hpos                 = mul                        (m_VP, P);                        // xform, input in world coords
		
		float3 positionView = mul(m_WV, P);
		o.fog       = ComputeExponentialFog(positionView);

		o.c0		= v.N.wwww;
		
		o.screenPos = ComputeScreenPos(o.hpos);
		o.screenPos.z = mul(m_V, P).z;
			
        return o;
}