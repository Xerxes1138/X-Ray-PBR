
#include	"common.h"

VertexOutput_GBUFFER main(VertexInput_Static v)
{
	float4 vertex = mul(m_WVP, v.position);
	float3 positionView = mul(m_V, v.position);
	float3 positionWorld = v.position;

    float3 normalWorld = v.normal * 2.0f - 1.0f;

    #ifdef USE_TBUMP
	    float3 tangentView = normalize(mul((float3x3)m_WV, v.tangent.xyz * 4.0f - 2.0f)); 
        float3 binormalView = normalize(mul((float3x3)m_WV, v.binormal.xyz * 4.0f - 2.0f));
	    float3 normalView = mul((float3x3)m_WV, v.normal.xyz * 4.0f - 2.0f);
		
        float3 T = unpack_bx4(v.tangent.xyz);  
        float3 B = unpack_bx4(v.binormal.xyz);
        float3 N = unpack_bx4(v.normal.xyz);

        float3x3 xform = mul((float3x3) m_WV, float3x3
		(
			T.x, B.x, N.x,
			T.y, B.y, N.y,
			T.z, B.z, N.z
		));
		
    #else
	    float3 normalView = mul((float3x3)m_WV, v.normal.xyz * 2.0f - 1.0f);
    #endif

    #ifdef USE_AREF
	    float4 positionScreen = ComputeScreenPos(vertex);
    #endif

	float2 textureCoord = unpack_tc_base(v.textureCoord, v.tangent.w, v.binormal.w);

    VertexOutput_GBUFFER vertexOuput = (VertexOutput_GBUFFER) 0;
    vertexOuput.vs_vertex = vertex;
    vertexOuput.vs_textureCoord = textureCoord;

    #if defined(USE_R2_STATIC_SUN) && !defined(USE_LM_HEMI) 
	    vertexOuput.vs_textureCoord.w	= v.color.w;
    #endif 

    vertexOuput.vs_positionView = float4(positionView, v.normal.w /* 3.0f*/);
	
	#ifdef USE_AREF
		vertexOuput.vs_positionScreen = positionScreen;
    #endif

    vertexOuput.vs_positionWorldAndDepth.xy = positionWorld.xz;
    vertexOuput.vs_positionWorldAndDepth.z = vertex.z;
    vertexOuput.vs_positionWorldAndDepth.w = vertex.w;

    #ifdef USE_TBUMP
        vertexOuput.vs_tangentView = xform[0];
        vertexOuput.vs_binormalView = xform[1];
        vertexOuput.vs_normalView = xform[2];
     #else
        vertexOuput.vs_normalView = normalView;
    #endif

	#ifdef STALKER_USE_LEGACY_RAIN
		vertexOuput.vs_normalWorld = ObjectSpaceToWorldSpaceNormal(normalWorld);
	#endif

#if defined(USE_PARALLAX) && defined(USE_TBUMP)
	vertexOuput.eye 			= mul(float3x3(T,B,N),-(positionWorld - eye_position));
#endif

#ifdef USE_TDETAIL
	vertexOuput.tcdbump			= textureCoord * dt_params; 
#endif

#ifdef USE_LM_HEMI
	vertexOuput.lmh 			= unpack_tc_lmap	(v.lightmapCoord);
#endif

    return vertexOuput;
}

FXVS;
