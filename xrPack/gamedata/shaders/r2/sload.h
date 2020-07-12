#ifndef SLOAD_H
#define SLOAD_H

#include "common.h"

struct surface_bumped
{
	float4	base;
	float3	specularColor;
	float3	normal;
	float	gloss;
	float	height;
	float	occlusion;
};

#ifdef DBG_TMAPPING
float4 tbase(float2 tc)
{
	float2 tile = max(ddx(tc),ddy(tc));
	return (1-max(tile.x,tile.y));
}
#else
float4 tbase(float2 tc)
{
	return tex2D(s_base, tc);
}
#endif

surface_bumped sload_i
(
#ifdef USE_FLAT
	p_flat I
#else
	p_bumped I
#endif
)
{
	float2 uv = I.tcdh;

	surface_bumped S = (surface_bumped)0;

	#ifdef USE_FLAT
		S.base = tex2D(s_base, uv);
		S.specularColor = (float3)0.04f;
		S.occlusion = 1;
		S.normal = float3(0, 0, 1);
	#else
		#ifdef USE_PARALLAX
			float height = tex2D(s_bumpX, uv).w;
			height = height * parallax.x + parallax.y;

			uv = uv + height * normalize(I.eye);
		#endif

		float4 surfaceData = tex2D(s_bumpX, uv); // R = Metallic, G = Occlusion, B = Height, A = Smoothness

		float4 surfaceDataBump = tex2D(s_bump, uv); // R = Smoothness, G = Occlusion, B = Normal Y, A = Normal X

		float4 baseColor = tex2D(s_base, uv);

		float3 normalTangent = UnpackNormal(surfaceDataBump);

		float metallic = 0 /*surfaceData.r*/;
		float occlusion = surfaceDataBump.g;
		float smoothness = surfaceDataBump.r;

		S.base.rgb = baseColor.rgb - metallic * baseColor.rgb;
		S.base.a = baseColor.a;
		S.specularColor = lerp((float3)0.04f, baseColor.rgb, metallic);
		S.normal = normalTangent;
		S.gloss = smoothness;
		S.height = surfaceData.a;
		S.occlusion = occlusion;
	#endif

	/*#ifdef USE_TDETAIL
		half4 detail = tex2D(s_detail, I.tcdbump);

		S.base.rgb = S.base.rgb * detail.rgb * 2;
		S.gloss = S.gloss * detail.w * 2;
	#endif*/

	return S;
}

surface_bumped sload
(
	#ifdef USE_FLAT
		p_flat I
	#else
		p_bumped I
	#endif
)
{
	surface_bumped S = sload_i(I);

	return S;
}
#endif
