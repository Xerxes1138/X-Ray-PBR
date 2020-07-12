#ifndef STALKER_RANDOM_H
#define STALKER_RANDOM_H

float WhiteNoise(float2 pos)
{
	return frac(sin(dot(pos.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float Dither8x8Bayer(int x, int y)
{
	const float dither[64] =
	{
		0,  32, 8,  40, 2,  34, 10, 42,
		48, 16, 56, 24, 50, 18, 58, 26,
		12, 44, 4,  36, 14, 46, 6,  38,
		60, 28, 52, 20, 62, 30, 54, 22,
		3,  35, 11, 43, 1,  33, 9,  41,
		51, 19, 59, 27, 49, 17, 57, 25,
		15, 47, 7,  39, 13, 45, 5,  37,
		63, 31, 55, 23, 61, 29, 53, 21
	};

	int r = y * 8 + x;

	return dither[r] / 64;
}

// [Jimenez 2014] "Next Generation Post Processing In Call Of Duty Advanced Warfare"  
float InterleavedGradientNoise(float2 pos)
{
	float3 magic = float3(0.06711056f, 0.00583715f, 52.9829189f);
	return frac(magic.z * frac(dot(pos.xy, magic.xy)));
}

float Hash(float n)
{
	return frac(sin(n) * 43758.5453123f);
}
#endif