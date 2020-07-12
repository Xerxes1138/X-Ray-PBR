#include	"common.h"
#include	"stalker_Skin.h"

VertexOutput_GBUFFER _main(VertexInput_Model v)
{
	float4 vertex = mul(m_WVP, v.position);
	float3 positionView = mul(m_WV, v.position); 
	float3 positionWorld = v.position;

    float3 normalWorld = v.normal * 2.0f - 1.0f;

    #ifdef USE_TBUMP
	    float3 normalView = mul((float3x3)m_V, v.normal * 2.0f - 1.0f);
	    float3 tangentView = normalize(mul((float3x3)m_V, v.tangent.xyz * 2.0f - 1.0f)); 
        float3 binormalView = normalize(mul((float3x3)m_V, v.binormal.xyz * 2.0f - 1.0f));

        float3 T = v.tangent.xyz;  
        float3 B = v.binormal.xyz;
        float3 N = v.normal.xyz;

        float3x3 xform = mul((float3x3) m_WV, float3x3
		(
			2.0f * T.x, 2.0f * B.x, 2.0f * N.x,
			2.0f * T.y, 2.0f * B.y, 2.0f * N.y,
			2.0f * T.z, 2.0f * B.z, 2.0f * N.z
		)
		);
		
    #else
	    float3 normalView = mul((float3x3)m_V, v.normal * 2.0f - 1.0f);
    #endif

    #ifdef USE_AREF
	    float4 positionScreen = ComputeScreenPos(vertex);
    #endif

	float2 textureCoord = v.textureCoord.xy;

    VertexOutput_GBUFFER vertexOuput = (VertexOutput_GBUFFER) 0;
    vertexOuput.vs_vertex = vertex;
    vertexOuput.vs_textureCoord = textureCoord;

    #if defined(USE_R2_STATIC_SUN) && !defined(USE_LM_HEMI)
	    vertexOuput.vs_textureCoord.w	= v.color.w;
    #endif

    vertexOuput.vs_positionView = float4(positionView, L_material.x /* 3.0f*/);
	
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
	vertexOuput.tcdbump		= textureCoord * dt_params; 
#endif

    return vertexOuput;
}

#ifdef 	SKIN_NONE
VertexOutput_GBUFFER main(VertexInput_Model v)
{
    return _main(v);
}
#endif

#ifdef 	SKIN_0
VertexOutput_GBUFFER main(VertexInput_Model_Skinned_0 v)
{
    return _main(skinning_0(v));
}
#endif

#ifdef	SKIN_1
VertexOutput_GBUFFER main(VertexInput_Model_Skinned_1 v)
{
    return _main(skinning_1(v));    
}
#endif

#ifdef	SKIN_2
VertexOutput_GBUFFER main(VertexInput_Model_Skinned_2 v)
{
    return _main(skinning_2(v));
}
#endif

