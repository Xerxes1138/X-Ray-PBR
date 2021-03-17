#include	"common.h"

VertexOutput_SIMPLE main(VertexInput_Simple v)
{
	float4 vertex = mul(m_WVP, v.position);

    VertexOutput_SIMPLE vertexOuput = (VertexOutput_SIMPLE) 0;
    vertexOuput.vs_vertex = vertex;
	
    return vertexOuput;
}


