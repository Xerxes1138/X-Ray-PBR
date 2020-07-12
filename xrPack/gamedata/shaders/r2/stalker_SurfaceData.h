#ifndef SurfaceData_H
#define SurfaceData_H

#include "common.h"

struct SurfaceData
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

SurfaceData surfaceDataInput
(
	VertexOutput_GBUFFER Input 
) 
{
	float2 uv = Input.vs_textureCoord; 

	SurfaceData surfaceData = (SurfaceData)0;
	
	float3 f0 = accurateLinearToSRGB((float3)0.04f);
		
	#ifdef USE_TBUMP
		#ifdef USE_PARALLAX
			float height = tex2D(s_bumpX, uv).w;
			height = height * parallax.x + parallax.y;

			uv = uv + height * normalize(Input.eye);
		#endif

		float4 materialData = tex2D(s_bumpX, uv); // R = 0, GB = Bump Errors, A = Height

		float4 materialDataBump = tex2D(s_bump, uv); // R = Smoothness, G = Occlusion, B = Normal Y, A = Normal X

		float4 baseColor = tex2D(s_base, uv);

		float3 normalTangent = UnpackNormal(materialDataBump);

		float metallic = 0.0f; // TODO : add a new RGBA texture in c++
		float occlusion = materialDataBump.g;
		float smoothness = materialDataBump.r;

		float oneMinusReflectivity = (1-f0) - metallic * (1-f0);  
   
		surfaceData.base.rgb = baseColor.rgb * oneMinusReflectivity;
		surfaceData.base.a = baseColor.a;
		surfaceData.specularColor = lerp(f0, baseColor.rgb, metallic);
		surfaceData.normal = normalTangent;
		surfaceData.gloss = smoothness;
		surfaceData.height = materialData.a;
		surfaceData.occlusion = occlusion;
	#else
		surfaceData.base = tex2D(s_base, uv);
		surfaceData.specularColor = f0;
		surfaceData.occlusion = 1;
		surfaceData.normal = float3(0, 0, 1);
	#endif

	/*#ifdef USE_TDETAIL
		half4 detail = tex2D(s_detail, I.tcdbump);

		S.base.rgb = S.base.rgb * detail.rgb * 2;
		S.gloss = S.gloss * detail.w * 2;
	#endif*/

	return surfaceData;
}

SurfaceData surfaceDataOutput
(
	VertexOutput_GBUFFER Input
)
{
	SurfaceData surfaceData = surfaceDataInput(Input);

	return surfaceData;
}
#endif
