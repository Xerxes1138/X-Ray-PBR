#include "common.h"

struct 	_in       
{
	float4 	p	: POSITION;
	float4  tc : TEXCOORD0;
};

struct         _input
{
	float4         hpos	: POSITION;
	float4         tc                : TEXCOORD0        ; 
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
_input main ( _in I )
{
	_input	O;

	O.hpos = I.p;
	O.tc = I.tc;

 	return O;
}