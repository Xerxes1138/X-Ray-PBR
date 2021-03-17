// INFO : material ID is setup in TextureDescrManager.cpp

#include "common.h"

uniform float4 consts;
#ifdef USE_WIND
    uniform float4 wave;
    uniform float4 dir2D; 
#endif
uniform float4 array[200] : register(c12);

VertexOutput_GBUFFER main(VertexInput_Detail v)
{
    // index
    int i = v.misc.w;
    float4 m0 = array[i + 0];
    float4 m1 = array[i + 1];
    float4 m2 = array[i + 2];
    float4 c0 = array[i + 3];

	// Transform pos to world coords
    float4 positionWorld;
    positionWorld.x = dot(m0, v.position);
    positionWorld.y = dot(m1, v.position);
    positionWorld.z = dot(m2, v.position);
    positionWorld.w = 1;

    #ifdef USE_WIND
        float base = m1.w;
        float dp = calc_cyclic(dot(positionWorld, wave));
        float H = positionWorld.y - base;
        float frac = v.misc.z * consts.x;
        float inten = H * dp;
        float2 result = calc_xz_wave(dir2D.xz * inten, frac);
        positionWorld = float4(positionWorld.x + result.x, positionWorld.y, positionWorld.z + result.y, 1.0f);
    #endif
    
	// Normal in world coords
    float3 normalWorld;
    normalWorld.x = positionWorld.x - m0.w;
    normalWorld.y = positionWorld.y - m1.w + 0.5f;
    normalWorld.z = positionWorld.z - m2.w;

    float4 vertex = mul(m_WVP, positionWorld);

    float3 positionView = mul(m_WV, positionWorld);
    float3 normalView = mul((float3x3) m_WV, normalize(normalWorld));

    #ifdef USE_AREF
        float4 positionScreen = ComputeScreenPos(vertex);
    #endif

    float2 textureCoord = float4((v.misc * consts).xyyy);

    VertexOutput_GBUFFER vertexOuput = (VertexOutput_GBUFFER)0;

    vertexOuput.vs_vertex = vertex;
    vertexOuput.vs_positionView = float4(positionView, /*saturate(distance((float3) 0.0f, v.position.xyz / 0.25f)) */ c0.w /* 3.0f*/);
	
	#ifdef USE_AREF
		vertexOuput.vs_positionScreen = positionScreen;
    #endif
	
    vertexOuput.vs_normalView = normalView;
    vertexOuput.vs_textureCoord = textureCoord;

    #if defined(USE_R2_STATIC_SUN)
        vertexOuput.vs_textureCoord.w	= c0.x;
    #endif

    vertexOuput.vs_positionWorldAndDepth.xy = positionWorld.xz;
    vertexOuput.vs_positionWorldAndDepth.zw = vertex.zw;

    return vertexOuput;
}
