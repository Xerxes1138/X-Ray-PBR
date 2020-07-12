#ifndef	SKIN_H
#define SKIN_H

#include "common.h"

/*struct VertexInput_Model
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float2 textureCoord : TEXCOORD0;
};*/

struct 	v_model_skinned_0
{
	float4 	P : POSITION;	// (float,float,float,1) - quantized	// short4
	float3	N : NORMAL;	// normal				// DWORD
	float3	T : TANGENT;	// tangent				// DWORD
	float3	B : BINORMAL;	// binormal				// DWORD
	float2	tc : TEXCOORD0;	// (u,v)				// short2
};

struct 	v_model_skinned_1   		// 24 bytes
{
	float4 	P : POSITION;	// (float,float,float,1) - quantized	// short4
	int4	N : NORMAL;	// (nx,ny,nz,index)			// DWORD
	float3	T : TANGENT;	// tangent				// DWORD
	float3	B : BINORMAL;	// binormal				// DWORD
	float2	tc : TEXCOORD0;	// (u,v)				// short2
};

struct 	v_model_skinned_2		// 28 bytes
{
	float4	P : POSITION;	// (float,float,float,1) - quantized	// short4
	float4 	N : NORMAL;	// (nx,ny,nz,weight)			// DWORD
	float3	T : TANGENT;	// tangent				// DWORD
	float3	B : BINORMAL;	// binormal				// DWORD
	int4 	tc : TEXCOORD0;	// (u,v, w=m-index0, z=m-index1)  	// short4
};

//////////////////////////////////////////////////////////////////////////////////////////
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
	float4 	P = float4(pos.xyz, 1.0f);
	return 	float4
	(
		dot(m0, P),
		dot(m1, P),
		dot(m2, P),
		1.0f
	);
}

VertexInput_Model skinning_0(v_model_skinned_0 v)
{
	// skinning
	VertexInput_Model 	o;
	o.position = float4(v.P.xyz, 1.0f);
	o.normal = unpack_normal(v.N);
	o.tangent = unpack_normal(v.T);
	o.binormal = unpack_normal(v.B);
	o.textureCoord = v.tc;
	return o;
}

VertexInput_Model skinning_1(v_model_skinned_1 v)
{
	// matrices
	int 	mid = v.N.w * (int)255;
	float4  m0 = sbones_array[mid + 0];
	float4  m1 = sbones_array[mid + 1];
	float4  m2 = sbones_array[mid + 2];

	// skinning
	VertexInput_Model 	o;
	o.position = skinning_pos(v.P, m0, m1, m2);
	o.normal = skinning_dir(v.N, m0, m1, m2);
	o.tangent = skinning_dir(v.T, m0, m1, m2);
	o.binormal = skinning_dir(v.B, m0, m1, m2);
	o.textureCoord = v.tc;
	return o;
}

VertexInput_Model skinning_2(v_model_skinned_2 v)
{
	// matrices
	int 	id_0 = v.tc.z;
	float4  m0_0 = sbones_array[id_0 + 0];
	float4  m1_0 = sbones_array[id_0 + 1];
	float4  m2_0 = sbones_array[id_0 + 2];
	int 	id_1 = v.tc.w;
	float4  m0_1 = sbones_array[id_1 + 0];
	float4  m1_1 = sbones_array[id_1 + 1];
	float4  m2_1 = sbones_array[id_1 + 2];

	// lerp
	float 	w = v.N.w;
	float4  m0 = lerp(m0_0, m0_1, w);
	float4  m1 = lerp(m1_0, m1_1, w);
	float4  m2 = lerp(m2_0, m2_1, w);

	// skinning
	VertexInput_Model 	o;
	o.position = skinning_pos(v.P, m0, m1, m2);
	o.normal = skinning_dir(v.N, m0, m1, m2);
	o.tangent = skinning_dir(v.T, m0, m1, m2);
	o.binormal = skinning_dir(v.B, m0, m1, m2);
	o.textureCoord = v.tc;
	return o;
}

#endif
