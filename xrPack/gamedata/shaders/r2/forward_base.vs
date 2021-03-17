#define USE_FORWARD_FOG

#include	"common.h"

VertexOutput_FORWARD main(VertexInput_Model v)
{
	float4 vertex = mul(m_WVP, v.position);
	float3 positionWorld = mul(m_W, half4(v.position.xyz, 1.0f));
    float3 normalWorld = normalize(mul((float3x3)m_W, unpack_bx2(v.normal.xyz))); //ObjectSpaceToWorldSpaceNormal(v.normal * 2.0f - 1.0f);

    #ifdef USE_AREF
	    float4 positionScreen = ComputeScreenPos(vertex);
    #endif

	float2 textureCoord = v.textureCoord.xy;

    VertexOutput_FORWARD vertexOuput = (VertexOutput_FORWARD) 0;
    vertexOuput.vs_vertex = vertex;
    vertexOuput.vs_textureCoord = textureCoord;
	
	#if defined(USE_R2_STATIC_SUN) && !defined(USE_LM_HEMI)
	    vertexOuput.vs_textureCoord.w	= v.color.w;
    #endif
	
    vertexOuput.vs_positionWorld = positionWorld;
	vertexOuput.vs_normalWorld = normalWorld;
	
	#ifdef USE_AREF
		vertexOuput.vs_positionScreen = positionScreen;
    #endif
	
	#ifdef USE_FORWARD_FOG
		float3 positionView = mul(m_WV, positionWorld);
		
		vertexOuput.vs_positionView = positionView;
	#endif
	
    return vertexOuput;
}


