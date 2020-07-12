
#include "common.h"

uniform float3x4 m_xform;
uniform float3x4 m_xform_v;
uniform float4 consts;
uniform float4 c_scale, c_bias, wind, wave;
uniform float2 c_sun;

VertexOutput_GBUFFER main(VertexInput_Tree v)
{
    float3 position = mul(m_xform, v.position);

    float base = m_xform._24;
    float dp = calc_cyclic(wave.w + dot(position, (float3) wave));
    float H = position.y - base;
    float frac = v.textureCoord.z * consts.x;
    float inten = H * dp;
    float2 result = calc_xz_wave(wind.xz * inten, frac);

    #ifdef USE_TREEWAVE
        result	= 0;
    #endif

	#ifdef USE_TREELEAF
		float3 L = v.position.xyz;

		float radius = length(float3(m_W._m01, m_W._m11, m_W._m21));
		L /= radius;
	
		//float occlusion = saturate(distance((float3)0.0f, l)); //1.0f - saturate(dot(n, l) / dot(l, l));
	#endif

    float4 positionWorld = float4(position.x + result.x, position.y, position.z + result.y, 1.0f);
    float2 textureCoord = (v.textureCoord * consts).xy;
	
	//#ifdef USE_TREELEAF
		//float ambientOcclusion = saturate(distance((float3)0.0f, L)) * v.normal.w * c_scale.w + c_bias.w;
	//#else
		float ambientOcclusion = (v.normal.w * c_scale.w + c_bias.w);
	//#endif
	
    float4 vertex = mul(m_VP, positionWorld);

    float3 positionView = mul(m_V, positionWorld);

    #ifdef USE_TBUMP
        float3 T = unpack_bx4(v.tangent);
        float3 B = unpack_bx4(v.binormal);
        float3 N = unpack_bx4(v.normal.xyz);
        float3x3 xform = mul((float3x3) m_xform_v, float3x3(T.x, B.x, N.x, T.y, B.y, N.y, T.z, B.z, N.z));
    #else
        float3 normalView = mul((float3x3) m_xform_v, unpack_bx2(v.normal.xyz));
    #endif

    #if defined(USE_AREF) && defined(USE_TREELEAF)
	    float4 positionScreen = ComputeScreenPos(vertex);
    #endif
	
	#ifdef USE_TREELEAF
		normalView = mul((float3x3)m_xform_v, v.position);
	#endif

	// Eye-space pos/normal
    VertexOutput_GBUFFER vertexOuput = (VertexOutput_GBUFFER)0;
    vertexOuput.vs_textureCoord = float4(textureCoord.xyyy);
    vertexOuput.vs_vertex = vertex;
    vertexOuput.vs_positionView = float4(positionView, ambientOcclusion);

    #if defined(USE_AREF) && defined(USE_TREELEAF)
		vertexOuput.vs_positionScreen = positionScreen;
    #endif

	vertexOuput.vs_positionWorldAndDepth.xy = positionWorld.xz;
	vertexOuput.vs_positionWorldAndDepth.zw = vertex.zw;

    #if defined(USE_R2_STATIC_SUN) && !defined(USE_LM_HEMI)
	    float 	suno 	= v.normal.w * c_sun.x + c_sun.y;
	    vertexOuput.textureCoord.w		= suno;
    #endif

    #ifdef USE_TBUMP
        vertexOuput.vs_tangentView = xform[0];
        vertexOuput.vs_binormalView = xform[1];
        vertexOuput.vs_normalView = xform[2];
    #else
         vertexOuput.vs_normalView = normalView;
    #endif

	#ifdef STALKER_USE_LEGACY_RAIN
		vertexOuput.vs_normalWorld = ObjectSpaceToWorldSpaceNormal(unpack_bx4(v.normal.xyz));
	#endif
	
    #ifdef 	USE_PARALLAX
	    vertexOuput.eye 			= mul		(float3x3(T,B,N),-(positionWorld - eye_position));
    #endif

    #ifdef 	USE_TDETAIL
	    vertexOuput.tcdbump		= vertexOuput.vs_textureCoord * dt_params;
    #endif

	return vertexOuput;
}
