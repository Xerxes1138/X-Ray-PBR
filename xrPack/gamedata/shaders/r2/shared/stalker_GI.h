#ifndef STALKER_GI_H
#define STALKER_GI_H

float3 SampleIBL(samplerCUBE cube, float3 dir)
{	
	#ifdef STALKER_LEGACY
		return accurateSRGBToLinear(texCUBE(cube, dir));
	#else
		return texCUBE(cube, dir);
	#endif
}

float3 SampleIBL(samplerCUBE cube, float3 dir, float mip)
{	
	#ifdef STALKER_LEGACY
		return accurateSRGBToLinear(texCUBElod(cube, float4(dir, mip)));
	#else
		return texCUBElod(cube, float4(dir, mip));
	#endif
}

// [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
// Changed by EPIC
float3 F_LazarovApprox( float3 specularColor, float roughness, float NdotV )
{
	const float4 c0 = { -1, -0.0275, -0.572, 0.022 };
	const float4 c1 = { 1, 0.0425, 1.04, -0.04 };
	float4 r = roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NdotV ) ) * r.x + r.y;
	float2 AB = float2( -1.04, 1.04 ) * a004 + r.zw;

	return specularColor * AB.x + AB.y;
}

float3 MultiBounce(float AO, float3 Albedo)
{
	float3 A = 2 * Albedo - 0.33;
	float3 B = -4.8 * Albedo + 0.64;
	float3 C = 2.75 * Albedo + 0.69;
	return max(AO, ((AO * A + B) * AO + C) * AO);
}

void ComputeImageBasedLighting(out float3 diffuseIBL, out float3 specularIBL, float3 N, float3 V, float4 blendFactor, float mip)
{
	float3 F = 1.0f;
	float3 R = reflect(V, N);
	
	R = RotateAroundYAxis(R, dx_SkyRotation.x);
	V = RotateAroundYAxis(V, dx_SkyRotation.x);
	N = RotateAroundYAxis(N, dx_SkyRotation.x);

	#ifdef STALKER_LEGACY	
		half3 diffuseCube0 = texCUBElod(s_diffuse_s0, float4(N, 0));
		half3 diffuseCube1 = texCUBElod(s_diffuse_s1, float4(N, 0));

		diffuseIBL = accurateSRGBToLinear(blendFactor.rgb * lerp(diffuseCube0, diffuseCube1, blendFactor.a));
		
		half3 specularCube0 = texCUBElod(s_specular_s0, float4(R, mip));
		half3 specularCube1 = texCUBElod(s_specular_s1, float4(R, mip));

		specularIBL = accurateSRGBToLinear(blendFactor.rgb * lerp(specularCube0, specularCube1, blendFactor.a));
	#else
		half3 diffuseCube0 = texCUBElod(s_diffuse_s0, float4(N, 0));
		half3 diffuseCube1 = texCUBElod(s_diffuse_s1, float4(N, 0));

		half3 skyColor = half3(0.25f, 0.5f, 1.0f);
		half3 groundColor = half3(0.1f, 0.125f, 0.0f);
		half3 hemisphereLight = lerp(groundColor, skyColor, N.y * 0.5f + 0.5f);
		
		diffuseIBL = blendFactor.rgb * lerp(diffuseCube0, diffuseCube1, blendFactor.a);
		
		half3 specularCube0 = texCUBElod(s_specular_s0, float4(R, mip));
		half3 specularCube1 = texCUBElod(s_specular_s1, float4(R, mip));

		specularIBL = blendFactor.rgb * lerp(specularCube0, specularCube1, blendFactor.a);
	#endif
}

void ComputeImageBasedLighting(out float3 diffuseIBL, out float3 specularIBL, float3 N, float3 V, float4 blendFactor, float3 diffuseColor, float smoothness, float ambientOcclusion)
{
	float3 R = reflect(V, N);
	
	R = RotateAroundYAxis(R, dx_SkyRotation.x);
	N = RotateAroundYAxis(N, dx_SkyRotation.x);
	
	float mip = (1 - smoothness) * STALKER_IBL_NUM_MIP; 
	
	#ifdef STALKER_MULTI_BOUNCE_SSAO_APPROX
		float3 occlusionDiffuse = MultiBounce(ambientOcclusion, diffuseColor);
	#else
		float3 occlusionDiffuse = ambientOcclusion;
	#endif
	
	half3 diffuseCube0 = SampleIBL(s_diffuse_s0, N);
	half3 diffuseCube1 = SampleIBL(s_diffuse_s1, N);

	diffuseIBL = blendFactor.rgb * lerp(diffuseCube0, diffuseCube1, blendFactor.a) * occlusionDiffuse;
		
	half3 specularCube0 = SampleIBL(s_specular_s0, R, mip);
	half3 specularCube1 = SampleIBL(s_specular_s1, R, mip);

	specularIBL = blendFactor.rgb * lerp(specularCube0, specularCube1, blendFactor.a) * ambientOcclusion;

}

void ComputeFogLighting(out float3 fogIBL, float3 V, float4 blendFactor)
{
	V = RotateAroundYAxis(V, dx_SkyRotation.x);

	half3 fogCube0 = SampleIBL(s_diffuse_s0, V);
	half3 fogCube1 = SampleIBL(s_diffuse_s1, V);

	fogIBL = blendFactor.rgb * lerp(fogCube0, fogCube1, blendFactor.a);
}

#endif