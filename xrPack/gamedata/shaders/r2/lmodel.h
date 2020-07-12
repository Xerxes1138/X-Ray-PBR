// Made by https://github.com/Xerxes1138

#ifndef	LMODEL_H
#define LMODEL_H

#include "common.h"
//

float3 EnergyConservingWrapDiffuseSSS(float3 xN, float3 yN, float3 zN, float3 L, float3 W)
{
	float x = (dot(xN, L) + W.x) / ((1 + W.x) * (1 + W.x));
	float y = (dot(yN, L) + W.y) / ((1 + W.y) * (1 + W.y));
	float z = (dot(zN, L) + W.z) / ((1 + W.z) * (1 + W.z));

	return saturate(float3(x, y, z));
}
//

// FROSTBITE
float3 F_Schlick(float3 f0, float f90, float u)
{
	return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

float V_SmithGGXCorrelated(float NdotL ,float NdotV ,float alphaG)
{
	// Original formulation of G_SmithGGX Correlated
	// lambda_v = (-1 + sqrt ( alphaG2 * (1 - NdotL2 ) / NdotL2 + 1)) * 0.5 f;
	// lambda_l = (-1 + sqrt ( alphaG2 * (1 - NdotV2 ) / NdotV2 + 1)) * 0.5 f;
	// G_SmithGGXCorrelated = 1 / (1 + lambda_v + lambda_l );
	// V_SmithGGXCorrelated = G_SmithGGXCorrelated / (4.0 f * NdotL * NdotV );

	// This is the optimize version
	alphaG = alphaG * alphaG;
	float alphaG2 = alphaG * alphaG;
	// Caution : the " NdotL *" and " NdotV *" are explicitely inversed , this is not a mistake .
	float Lambda_GGXV = NdotL * sqrt((-NdotV * alphaG2 + NdotV) * NdotV + alphaG2);
	float Lambda_GGXL = NdotV * sqrt((-NdotL * alphaG2 + NdotL) * NdotL + alphaG2);

	return 0.5f / (Lambda_GGXV + Lambda_GGXL);
}

float D_GGX(float NdotH, float m)
{
	// Divide by PI is apply later
	m = m * m;
	float m2 = m * m;
	float f = (NdotH * m2 - NdotH) * NdotH + 1.0f;
	return /*(1.0f / STALKER_PI) */ m2 / (f * f  + 1e-5f);
}

float DisneyDiffuse(float NdotV, float NdotL, float LdotH, float linearRoughness)
{
   float fd90 = 0.5f + 2.0f * LdotH * LdotH * linearRoughness;
   float f0 = 1.0f;
   float lightScatter = (f0 + (fd90 - f0) * pow(1.0f - NdotL, 5.0f));
   float viewScatter = (f0 + (fd90 - f0) * pow(1.0f - NdotV, 5.0f));

   return lightScatter * viewScatter;
}

float Fr_DisneyDiffuse(float NdotV, float NdotL, float LdotH, float linearRoughness)
{
   float energyBias = lerp (0.0f, 0.5f, linearRoughness);
   float energyFactor = lerp (1.0f, 1.0f / 1.51f, linearRoughness);
   float fd90 = energyBias * 0.5f + 2.0f * LdotH * LdotH * linearRoughness;
   float3 f0 = float3 (1.0f, 1.0f, 1.0f);
   float lightScatter = F_Schlick(f0, fd90, NdotL).r;
   float viewScatter = F_Schlick(f0, fd90, NdotV).r;

   return lightScatter * viewScatter * energyFactor;
}
//

float3 BSDF(float3 N, float3 V, float3 L, float3 specularColor, float smoothness, bool isDiffuse = true)
{
	// Moving to the Next Generation - The Rendering Technology of Ryse
	//float roughness = pow(1.0f - smoothness * 0.7f, 6.0f);//max(1.0f - smoothness, 0.05f);
	
	float roughness = max(1.0f - smoothness, 0.05f);
		
	float linearRoughness = roughness;
	
	float3 h	= L + V;
	float3 H	= normalize(h);
		
	float NdotH = saturate(dot(N,H));
	float NdotL = saturate(dot(N,L));
	float NdotV = saturate(dot(N,V));
	float LdotH = saturate(dot(L,H));
	
	float D = D_GGX(NdotH, roughness);
	float Vis = V_SmithGGXCorrelated(NdotL, NdotV, roughness);
	
	float fd90 = 0.5f + 2.0 * LdotH * LdotH * roughness;
	float3 f0 = specularColor;
	float3 F = f0 + (1.0f - f0) * pow(1.0f - LdotH, 5.0f) /*F_Schlick((float3)0.04f, fd90, LdotH)*/;
	
	float3 Specular = D * F * Vis /* STALKER_PI*/;
	
	float3 Diffuse = 1.0f /* STALKER_PI*/;
	
	if(0)
	{
		Diffuse = Fr_DisneyDiffuse(NdotV, NdotL, LdotH , linearRoughness);
	}

	return (isDiffuse ? Diffuse : Specular) * NdotL /* STALKER_PI*/;
}

void plight_infinity(out float3 diffuseLight, out float3 specularLight, float3 specularColor, float smoothness, float3 pos, float3 normal, float3 light_direction)
{
	float3 N		= normalize(normal);
	float3 V 	= normalize(pos);
	float3 L 	= -light_direction;
	
	diffuseLight = BSDF(N, -V, L, specularColor, smoothness);
	specularLight  = BSDF(N, -V, L, specularColor, smoothness, false);
}

void plight_local(out float3 diffuseLight, out float3 specularLight, float3 specularColor, float smoothness, float3 pos, float3 normal, float3 light_position, float light_range_rsq, out float rsqr)  
{
	float3 N	= normalize(normal);	
	float3 L2P = pos - light_position;
	float3 V = normalize(pos);
	float3 L = -normalize(L2P);

	rsqr = dot(L2P, L2P);
	
	float attenuation = saturate( 1.0f - rsqr * light_range_rsq);
	
	if(1)
	{
		attenuation = Square(saturate( 1.0f - Square(rsqr * light_range_rsq)));
	}

	diffuseLight = BSDF(N, -V, L, specularColor, smoothness) * attenuation;
	specularLight  = BSDF(N, -V, L, specularColor, smoothness, false) * attenuation;
}

float4	blendp	(float4	value, float4 	tcp)    		{
	#ifndef FP16_BLEND  
		value 	+= (float4)tex2Dproj 	(s_accumulator, tcp); 	// emulate blend
	#endif
	return 	value	;
}
float4	blend	(float4	value, float2 	tc)			{
	#ifndef FP16_BLEND  
		value 	+= (float4)tex2D 	(s_accumulator, tc); 	// emulate blend
	#endif
	return 	value	;
}

#endif
