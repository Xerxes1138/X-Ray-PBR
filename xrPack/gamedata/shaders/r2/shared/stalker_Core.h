#ifndef STALKER_CORE_H
#define STALKER_CORE_H

// SAMPLER
uniform sampler2D s_bluenoise;

uniform samplerCUBE s_diffuse_s0;
uniform samplerCUBE s_diffuse_s1;

uniform samplerCUBE s_specular_s0;
uniform samplerCUBE s_specular_s1;

uniform sampler2D s_AO;

// CONSTANTS
#define GOLDEN_RATIO 1.6180339887498948482f
	
uniform	float4 dx_ScreenParams; // x = Screen width, y = Screen height, z = 1.0f / x, w = 1.0f / y

uniform	float4 dx_WeatherParams; // x = Rain density, y = 0.0f, z = 0.0f, w = 0.0f
uniform float4 dx_SkyRotation;
uniform float4 dx_AmbientColor;
uniform float4 dx_SkyColor;

uniform float4 dx_Pos_Decompression_Params;

uniform float4 dx_Projection_Params;
uniform float4 dx_Viewport_Params;

uniform float4 dx_SSAO_Params;
uniform float4 dx_SSRS_Params;
uniform float4 dx_SSR_Params;
uniform float4 dx_TAA_Params;
uniform float4 dx_Tonemap_Params;

uniform	float3x4 dx_matrix_ViewToWorld;
uniform float3x4 dx_matrix_CameraToWorld;

uniform	float4x4 dx_matrix_CameraProjection;
uniform	float4x4 dx_matrix_Projection;
uniform	float4x4 dx_matrix_PreviousProjection;
uniform float4x4 dx_matrix_InverseProjection;
uniform float4x4 dx_matrix_InverseViewProjection;
uniform float4x4 dx_matrix_ViewProjection;

#define STALKER_SCREEN_SIZE dx_ScreenParams
#define STALKER_PI 3.14159265359
#define STALKER_TWO_PI STALKER_PI * 2.0f

#define STALKER_SUN_WAVELENGTH float3(0.2737f, 0.3474f, 0.4211f)

#define STALKER_IBL_NUM_MIP 8
#endif