#ifndef STALKER_UTILS_H
#define STALKER_UTILS_H

float Luminance(float3 rgb)
{
    return dot(rgb, float4(0.0396819152, 0.458021790, 0.00609653955, 1.0));
}

float3 ObjectSpaceToWorldSpaceNormal(float3 dir)
{
	return normalize(mul((float3x3)m_W, dir));
}

float Square(float x)
{
	return x * x;
}

float ComputeLegacyFog(float3 position /*View Space*/)
{
	float distance = length(position);
	return saturate(distance * fog_params.w + fog_params.x);
}

float ComputeExponentialFog(float3 position /*View Space*/)
{
	float distance = length(position);
	return 1.0f - exp(-distance * STALKER_FOG_EXP);
}

float ComputeHeightFog(float3 position /*World Space*/, float3 cameraPosition /*World Space*/, float rainDensity)
{
    float fogHeight = (position.y - (cameraPosition.y + STALKER_FOG_HEIGHT_OFFSET)) / STALKER_FOG_HEIGHT;
	fogHeight = exp(-fogHeight /* STALKER_FOG_HEIGHT_DENSITY*/);
	
	return lerp(fogHeight, 1.0f, rainDensity);
}

float3 UnpackNormal(float4 packednormal)
{
    float3 normal;
    normal.xy = packednormal.wz * 2 - 1;
    normal.z = sqrt(1 - saturate(dot(normal.xy, normal.xy)));
    return normal;
}

// [Lagarde, De Rousiers SIGGRAPH 2014] "Moving Frostbite to Physically Based Rendering"
float3 accurateSRGBToLinear(float3  sRGBCol)
{
    float3  linearRGBLo   = sRGBCol / 12.92f;
    float3  linearRGBHi   = pow(( sRGBCol + 0.055f) / 1.055f,  2.4f);
    float3  linearRGB     = (sRGBCol  <= 0.04045f) ? linearRGBLo : linearRGBHi;
    return  linearRGB;
}

float3 accurateLinearToSRGB(float3  linearCol)
{
    float3 sRGBLo = linearCol * 12.92f;
    float3 sRGBHi = (pow(abs(linearCol), 1.0f/2.4f) * 1.055f)  - 0.055f;
    float3 sRGB    = (linearCol  <= 0.0031308f) ? sRGBLo : sRGBHi;
    return  sRGB;
}
//


//
// M matrix, for encoding
const static float3x3 M = float3x3(
    0.2209, 0.3390, 0.4184,
    0.1138, 0.6780, 0.7319,
    0.0102, 0.1130, 0.2969);

// Inverse M matrix, for decoding
const static float3x3 InverseM = float3x3(
    6.0013,    -2.700,    -1.7995,
    -1.332,    3.1029,    -5.7720,
    .3007,    -1.088,    5.6268);    

float4 LogLuvEncode(in float3 vRGB)
{
    float4 vResult;
    float3 Xp_Y_XYZp = mul(vRGB, M);
    Xp_Y_XYZp = max(Xp_Y_XYZp, float3(1e-6, 1e-6, 1e-6));
    vResult.xy = Xp_Y_XYZp.xy / Xp_Y_XYZp.z;
    float Le = 2 * log2(Xp_Y_XYZp.y) + 127;
    vResult.w = frac(Le);
    vResult.z = (Le - (floor(vResult.w*255.0f))/255.0f)/255.0f;
    return vResult;
}

float3 LogLuvDecode(in float4 vLogLuv)
{
    float Le = vLogLuv.z * 255 + vLogLuv.w;
    float3 Xp_Y_XYZp;
    Xp_Y_XYZp.y = exp2((Le - 127) / 2);
    Xp_Y_XYZp.z = Xp_Y_XYZp.y / vLogLuv.y;
    Xp_Y_XYZp.x = vLogLuv.x * Xp_Y_XYZp.z;
    float3 vRGB = mul(Xp_Y_XYZp, InverseM);
    return max(vRGB, 0);
}
//

float4 ComputeScreenPos(float4 pos) 
{
	float scale = -1.0f; // DX9 
    float4 o = pos * 0.5f;
    o.xy = float2(o.x, o.y * scale) + o.w;
    o.zw = pos.zw;
    return o;
}

bool ComputeCheckerboard(int2 pos)
{
	bool checkerboard = fmod(pos.x, 2.0f) == fmod(pos.y, 2.0f);
	
	return checkerboard;
}

float ColorDiffBlend (float3 a, float3 b)
{
	float3 diff = a - b;
	float length = sqrt(dot(diff, diff));
	return 1.0f / (length + 1e-3f);
}

//
float computeEV100(float aperture, float shutterTime, float ISO)
{
	// EV number is defined as:
	// 2^ EV_s = N^2 / t and EV_s = EV_100 + log2 (S /100)
	// This gives
	// EV_s = log2 (N^2 / t)
	// EV_100 + log2 (S /100) = log2 (N^2 / t)
	// EV_100 = log2 (N^2 / t) - log2 (S /100)
	// EV_100 = log2 (N^2 / t . 100 / S)
	return log2(Square(aperture) / shutterTime * 100.0f / ISO);
}

float computeEV100FromAvgLuminance(float avgLuminance)
{
	// We later use the middle gray at 12.7% in order to have
	// a middle gray at 18% with a sqrt (2) room for specular highlights
	// But here we deal with the spot meter measuring the middle gray
	// which is fixed at 12.5 for matching standard camera
	// constructor settings (i.e. calibration constant K = 12.5)
	// Reference : http :// en. wikipedia . org / wiki / Film_speed
	return log2(avgLuminance * 100.0f / 12.5f);
}

float convertEV100ToExposure(float EV100)
{
	// Compute the maximum luminance possible with H_sbs sensitivity
	// maxLum = 78 / ( S * q ) * N^2 / t
	// = 78 / ( S * q ) * 2^ EV_100
	// = 78 / (100 * 0.65) * 2^ EV_100
	// = 1.2 * 2^ EV
	// Reference : http :// en. wikipedia . org / wiki / Film_speed
	float maxLuminance = 1.2f * pow(2.0f, EV100);
	return 1.0f / maxLuminance;
}


//



//=================================================================================================
//
//  Baking Lab
//  by MJP and David Neubelt
//  http://mynameismjp.wordpress.com/
//
//  All code licensed under the MIT license
//
//=================================================================================================

// The code in this file was originally written by Stephen Hill (@self_shadow), who deserves all
// credit for coming up with this fit and implementing it. Buy him a beer next time you see him. :)

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
static const float3x3 ACESInputMat =
{
    {0.59719, 0.35458, 0.04823},
    {0.07600, 0.90834, 0.01566},
    {0.02840, 0.13383, 0.83777}
};

// ODT_SAT => XYZ => D60_2_D65 => sRGB
static const float3x3 ACESOutputMat =
{
    { 1.60475, -0.53108, -0.07367},
    {-0.10208,  1.10813, -0.00605},
    {-0.00327, -0.07276,  1.07602}
};

float3 RRTAndODTFit(float3 v)
{
    float3 a = v * (v + 0.0245786f) - 0.000090537f;
    float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

float3 ACESFitted(float3 color)
{
    color = mul(ACESInputMat, color);

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = mul(ACESOutputMat, color);

    // Clamp to [0, 1]
    color = saturate(color);

    return color;
}

float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x*(a*x+b))/(x*(c*x+d)+e));
}

// Applies the filmic curve from John Hable's presentation
float3 ToneMapFilmicALU(in float3 color)
{
    color = max(0, color - 0.004f);
    color = (color * (6.2f * color + 0.5f)) / (color * (6.2f * color + 1.7f)+ 0.06f);
    return color;
}

// Sebastien Lagarde "Physically based wet surfaces"
void DoWetProcess(inout float3 diffuseColor, inout float smoothness, float metallic, float wetLevel)
{
	float porosity = saturate(((1 - smoothness) - 0.5) / 0.4);
	float factor = lerp(1, 0.2, (1 - metallic) * porosity * wetLevel);

	diffuseColor *= factor;
	smoothness = min(smoothness * lerp(1.0, 2.5, wetLevel), 1.0);
}

float3 ComputeRipple(sampler2D tex, float2 UV, float CurrentTime, float Weight)
{
	float4 Ripple = tex2D(tex, UV);
	Ripple.yz = Ripple.yz * 2 - 1; // Decompress perturbation

	float DropFrac = frac(Ripple.w + CurrentTime); // Apply time shift
	float TimeFrac = DropFrac - 1.0f + Ripple.x;
	float DropFactor = saturate(0.2f + Weight * 0.8f - DropFrac);
	float FinalFactor = DropFactor * Ripple.x *
		sin(clamp(TimeFrac * 9.0f, 0.0f, 3.0f) * STALKER_PI);

	return float3(Ripple.yz * FinalFactor * 0.35f, 1.0f);
}


float4 SSSAtenuation(float distance, float3 sssColor)
{
		float3 r = distance / (1e-3f + sssColor);
		float3 rr = r*r;
		float3 sssAttenuation = exp2(-rr);

		return float4(sssAttenuation, 1.0f);
}

float3 RotateAroundYAxis(float3 v, float deg)
   {
        float alpha = deg /* STALKER_PI / 180.0*/;
        float sina, cosa;
        sincos(alpha, sina, cosa);
        float2x2 m = float2x2(cosa, -sina, sina, cosa);
        return float3(mul(m, v.xz), v.y).xzy;
    }
	
float LinearDepth(float z)
{
	return dx_Viewport_Params.x * dx_Viewport_Params.y / (dx_Viewport_Params.y - z * (dx_Viewport_Params.y - dx_Viewport_Params.x));
}

float3 InverseToneMapping(float3 color)
{
	return (0.22f * color / (1.0275f - color));
}

half DeriveSpecularOcclusion(half fNdotV, half aoAmount, half smoothness)
{
	// Derive specular occlusion term form ambient occlusion:
	//   Rough surfaces receive full ambient occlusion
	//   Smooth surfaces get more occlusion at grazing angles and less at normal incidence
	return saturate(pow(fNdotV + aoAmount, smoothness) - 1 + aoAmount);
}

float ComputeSpecOcclusion( float NdotV , float AO , float roughness )
{
	return saturate (pow(NdotV + AO , roughness) - 1 + AO);
}

float4 TangentToWorld(float3 N, float4 H)
{
	float3 UpVector = abs(N.z) < 0.999f ? float3(0.0f, 0.0f, 1.0f) : float3(1.0f, 0.0f, 0.0f);
	float3 T = normalize( cross( UpVector, N ) );
	float3 B = cross( N, T );
				 
	return float4((T * H.x) + (B * H.y) + (N * H.z), H.w);
}	
#endif