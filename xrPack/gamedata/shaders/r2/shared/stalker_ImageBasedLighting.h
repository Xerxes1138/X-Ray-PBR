#ifndef STALKER_IBL_H
#define STALKER_IBL_H

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
float radicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
 }

float2 hammersley2d(uint i, uint N)
{
    return float2(float(i)/float(N), radicalInverse_VdC(i));
}

////////

float3 hemisphereSample_uniform(float u, float v)
{
    float phi = v * 2.0 * STALKER_PI;
    float cosTheta = 1.0 - u;
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    return float3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

float3 hemisphereSample_cos(float u, float v)
{
    float phi = v * 2.0 * STALKER_PI;
    float cosTheta = sqrt(1.0 - u);
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    return float3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

float4 CosineSampleHemisphere(float2 E)
{
	float Phi = 2.0f * STALKER_PI * E.y;
	float CosTheta = sqrt(max(0.0f, 1.0f - E.x));
	float SinTheta = sqrt(E.x);

	float3 L;
	L.x = SinTheta * cos(Phi);
	L.y = SinTheta * sin(Phi);
	L.z = CosTheta;

	float PDF = CosTheta / STALKER_PI;

	return float4(L, PDF);
}

float4 UniformSampleCone( float2 E, float CosThetaMax )
{
	float Phi = 2 * STALKER_PI * E.x;
	float CosTheta = lerp( CosThetaMax, 1, E.y );
	float SinTheta = sqrt( 1 - CosTheta * CosTheta );

	float3 L;
	L.x = SinTheta * cos( Phi );
	L.y = SinTheta * sin( Phi );
	L.z = CosTheta;

	float PDF = 1.0 / ( 2 * STALKER_PI * (1 - CosThetaMax) );

	return float4( L, PDF );
}

// Brian Karis, Epic Games "Real Shading in Unreal Engine 4"
float4 ImportanceSampleGGX(float2 Xi, float Roughness)
{
	float m = Roughness * Roughness;
	float m2 = m * m;
		
	float Phi = 2.0f * STALKER_PI * Xi.x;
				 
	float CosTheta = sqrt((1.0f - Xi.y) / (1.0f + (m2 - 1.0f) * Xi.y));
	float SinTheta = sqrt(max(1e-5f, 1.0f - CosTheta * CosTheta));
				 
	float3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;
		
	float d = (CosTheta * m2 - CosTheta) * CosTheta + 1.0f;
	float D = m2 / (STALKER_PI * d * d);
	float pdf = D * CosTheta;

	return float4(H, pdf); 
}
#endif