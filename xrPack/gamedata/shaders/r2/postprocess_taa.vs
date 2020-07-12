#include "common.h"

struct 	TAAVertexInput
{
    float4 p : POSITION;
};

struct TAAVertexOuput
{
	float4 hpos	: POSITION;
	float2 tc0 : TEXCOORD0;
	float2 tc1 : TEXCOORD1;
};

TAAVertexOuput main(TAAVertexInput Input)
{
	TAAVertexOuput 	O;
    O.hpos = float4(Input.p.x, -Input.p.y, 0, 1);
    O.tc0 = Input.p.zw;
    O.tc1 = Input.p.zw;
    return O;
}
