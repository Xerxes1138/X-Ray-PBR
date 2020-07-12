#include "common.h"

struct VertexInput_Particle
{
	float4 position				: POSITION;
	float3 normal				: NORMAL;
	float2 textureCoord			: TEXCOORD0;
	float4 vertexColor			: COLOR0;
};

struct VertexOuput_Particle
{
	float4 vs_vertex			: POSITION;
	float4 vs_positionScreen	: TEXCOORD0;
	float2 vs_textureCoord		: TEXCOORD1;
	float4 vs_vertexColor		: TEXCOORD2;
	float3 vs_normalView : TEXCOORD3;
};

VertexOuput_Particle main (VertexInput_Particle v)
{
	float4 vertex = mul(m_WVP, v.position);
	float3 positionView = mul(m_V, v.position);
	float4 positionScreen = ComputeScreenPos(vertex);

	VertexOuput_Particle vertexOuput = (VertexOuput_Particle) 0;
	vertexOuput.vs_vertex = vertex;
	vertexOuput.vs_textureCoord = v.textureCoord;				
	vertexOuput.vs_vertexColor = v.vertexColor;
	vertexOuput.vs_positionScreen = float4(positionScreen.xy, positionView.z, positionScreen.w);
	vertexOuput.vs_normalView = mul((float3x3)m_WV, v.normal);
	return vertexOuput;
}
