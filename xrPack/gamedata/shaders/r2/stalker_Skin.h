#ifndef	STALKER_SKIN_H
#define STALKER_SKIN_H

#include "common.h"

struct 	VertexInput_Model_Skinned_0
{
	float4 	position : POSITION;
	float3	normal : NORMAL;
	float3	tangent : TANGENT;
	float3	binormal : BINORMAL;
	float2	textureCoord : TEXCOORD0;
};

struct 	VertexInput_Model_Skinned_1
{
	float4 	position : POSITION;
	int4	normal : NORMAL;
	float3	tangent : TANGENT;
	float3	binormal : BINORMAL;
	float2	textureCoord : TEXCOORD0;
};

struct 	VertexInput_Model_Skinned_2
{
	float4	position : POSITION;
	float4 	normal : NORMAL;
	float3	tangent : TANGENT;
	float3	binormal : BINORMAL;
	int4 	textureCoord : TEXCOORD0;
};

uniform float4 	sbones_array[64 * 3];

float3 	skinning_dir(float3 dir, float3 m0, float3 m1, float3 m2)
{
	float3 	U = unpack_normal(dir);
	return 	float3
	(
		dot(m0, U),
		dot(m1, U),
		dot(m2, U)
	);
}
float4 	skinning_pos(float4 pos, float4 m0, float4 m1, float4 m2)
{
	float4 	position = float4(pos.xyz, 1.0f);
	return 	float4
	(
		dot(m0, position),
		dot(m1, position),
		dot(m2, position),
		1.0f
	);
}

VertexInput_Model skinning_0(VertexInput_Model_Skinned_0 v)
{
	// skinning
	VertexInput_Model 	o;
	o.position = float4(v.position.xyz, 1.0f);
	o.normal = unpack_normal(v.normal);
	o.tangent = unpack_normal(v.tangent);
	o.binormal = unpack_normal(v.binormal);
	o.textureCoord = v.textureCoord;
	return o;
}

VertexInput_Model skinning_1(VertexInput_Model_Skinned_1 v)
{
	// matrices
	int 	mid = v.normal.w * (int)255;
	float4  m0 = sbones_array[mid + 0];
	float4  m1 = sbones_array[mid + 1];
	float4  m2 = sbones_array[mid + 2];

	// skinning
	VertexInput_Model 	o;
	o.position = skinning_pos(v.position, m0, m1, m2);
	o.normal = skinning_dir(v.normal, m0, m1, m2);
	o.tangent = skinning_dir(v.tangent, m0, m1, m2);
	o.binormal = skinning_dir(v.binormal, m0, m1, m2);
	o.textureCoord = v.textureCoord;
	return o;
}

VertexInput_Model skinning_2(VertexInput_Model_Skinned_2 v)
{
	// matrices
	int 	id_0 = v.textureCoord.z;
	float4  m0_0 = sbones_array[id_0 + 0];
	float4  m1_0 = sbones_array[id_0 + 1];
	float4  m2_0 = sbones_array[id_0 + 2];
	int 	id_1 = v.textureCoord.w;
	float4  m0_1 = sbones_array[id_1 + 0];
	float4  m1_1 = sbones_array[id_1 + 1];
	float4  m2_1 = sbones_array[id_1 + 2];

	// lerp
	float 	w = v.normal.w;
	float4  m0 = lerp(m0_0, m0_1, w);
	float4  m1 = lerp(m1_0, m1_1, w);
	float4  m2 = lerp(m2_0, m2_1, w);

	// skinning
	VertexInput_Model 	o;
	o.position = skinning_pos(v.position, m0, m1, m2);
	o.normal = skinning_dir(v.normal, m0, m1, m2);
	o.tangent = skinning_dir(v.tangent, m0, m1, m2);
	o.binormal = skinning_dir(v.binormal, m0, m1, m2);
	o.textureCoord = v.textureCoord;
	return o;
}

#endif
