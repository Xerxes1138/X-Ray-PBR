#include "common.h"

VertexOuput main(VertexInput v)
{	
	float4 vertex = mul(m_WVP, v.position);
	float3 positionView = mul(m_V, v.position);
	float3 normalView = mul((float3x3)m_V, v.normal * 2.0f - 1.0f);
	float3 tangentView = normalize(mul((float3x3)m_V, v.tangent.xyz * 2.0f - 1.0f)); 
    float3 binormalView = normalize(mul((float3x3)m_V, v.binormal.xyz * 2.0f - 1.0f));
	float4 screenPos = ComputeScreenPos(vertex);
	float2 textureUV = unpack_tc_base(v.textureUV, v.tangent.w, v.binormal.w);

	VertexOuput vertexOuput = (VertexOuput)0;
	vertexOuput.vs_vertex = mul(m_WVP, v.position);
	vertexOuput.vs_positionView = positionView;
	vertexOuput.vs_normalView = normalView;
	vertexOuput.vs_tangentView = tangentView; 
    vertexOuput.vs_binormalView = binormalView; 
	vertexOuput.vs_screenPos = screenPos;
	vertexOuput.vs_textureUV = textureUV;
	vertexOuput.vs_position = vertex;
	vertexOuput.vs_normalWorld = ObjectSpaceToWorldSpaceNormal(v.normal * 2.0f - 1.0f);
	return vertexOuput;
}