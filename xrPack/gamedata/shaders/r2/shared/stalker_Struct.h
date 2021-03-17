#ifndef STALKER_STRUCT_H
#define STALKER_STRUCT_H

struct VertexInput_Simple
{
	float4 position : POSITION;
};

struct VertexInput_Static
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float2 textureCoord : TEXCOORD0;
	float2 lightmapCoord : TEXCOORD1;
	float4 color : COLOR;
};

struct VertexInput_Model
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 textureCoord : TEXCOORD0;
};

struct VertexInput_Tree
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float4 textureCoord : TEXCOORD0;
};

struct VertexInput_Detail
{
	float4 position : POSITION;
	float4 color : COLOR;
	int4 misc : TEXCOORD0;
};

struct VertexOutput_SIMPLE
{
	float4 vs_vertex : POSITION;
};

struct VertexOutput_GBUFFER
{
	float4 vs_vertex : POSITION;

#if defined(USE_R2_STATIC_SUN) && !defined(USE_LM_HEMI)
	float4 vs_textureCoord : TEXCOORD0;
#else
	float2 vs_textureCoord : TEXCOORD0;
#endif

	float4 vs_positionView : TEXCOORD1;

	float4 vs_positionWorldAndDepth : TEXCOORD2;
	
	/*#ifdef STALKER_USE_LEGACY_RAIN
		float3 vs_normalWorld : TEXCOORD3;
	#endif*/
	
	#ifdef USE_AREF
	    float4 vs_positionScreen : TEXCOORD3;
    #endif

#ifdef USE_TBUMP
	float3 vs_normalView : TEXCOORD4;
	float3 vs_tangentView : TEXCOORD5;
	float3 vs_binormalView : TEXCOORD6;

	#ifdef USE_PARALLAX
		float3 eye : TEXCOORD7;
	#endif

	#ifdef USE_TDETAIL
		float2 tcdbump : TEXCOORD8;
	#endif

	#ifdef USE_LM_HEMI
		float2 lmh : TEXCOORD9;
	#endif
#else
	
	float3 vs_normalView : TEXCOORD5;

	#ifdef USE_TDETAIL
		float2 tcdbump : TEXCOORD6;
	#endif

	#ifdef USE_LM_HEMI
		float2 lmh : TEXCOORD7;
	#endif
#endif
};

struct VertexOutput_FORWARD
{
	float4 vs_vertex : POSITION;

#if defined(USE_R2_STATIC_SUN) && !defined(USE_LM_HEMI)
	float4 vs_textureCoord : TEXCOORD0;
#else
	float2 vs_textureCoord : TEXCOORD0;
#endif
	float3 vs_positionWorld : TEXCOORD1;

	float3 vs_normalWorld : TEXCOORD2;
	
	#ifdef USE_AREF
	    float4 vs_positionScreen : TEXCOORD3;
    #endif
	
	#ifdef USE_LM_HEMI
		float2 lmh : TEXCOORD4;
	#endif
	
	#ifdef USE_FORWARD_FOG
	float3 vs_positionView : TEXCOORD5;
		//float vs_vertexFog : TEXCOORD6;
	#endif
};

struct AccumVertexOuput     
{
	float2 tc : TEXCOORD0;
	float4 tcJ : TEXCOORD1;
};

struct AccumBaseVertexOuput     
{
	float4 tc : TEXCOORD0;
	float4 tcJ : TEXCOORD1;
};

struct VertexInput
{
	float4 position			: POSITION;
	float3 normal 			: NORMAL;
	float4 tangent			: TANGENT;
	float4 binormal			: BINORMAL;
	float2 textureUV		: TEXCOORD0;
};

struct VertexOuput
{
	float4 vs_vertex		: POSITION;
	float3 vs_positionView	: TEXCOORD0;
	float3 vs_normalView 	: TEXCOORD1;
	float3 vs_tangentView	: TEXCOORD2;
	float3 vs_binormalView	: TEXCOORD3;
	float4 vs_screenPos		: TEXCOORD4;
	float2 vs_textureUV		: TEXCOORD5;
	float4 vs_position		: TEXCOORD6;
	float3 vs_normalWorld	: TEXCOORD7;
};

#endif