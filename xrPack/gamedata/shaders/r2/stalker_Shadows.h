#ifndef	STALKER_SHADOW_H
#define STALKER_SHADOW_H

half shadow_jitter(float4 tc, float2 tcJ)
{
	const int num = 4; 
	
	const float 	scale 	= (6.0f / float(SMAP_size));
	
	float4 blueNoise = tex2D(s_bluenoise, tcJ);
	
	#ifdef STALKER_USE_TAA
		blueNoise.x = fmod(blueNoise.x + (GOLDEN_RATIO) * (timers.x % 100), 1);
		blueNoise.y = fmod(blueNoise.y + (GOLDEN_RATIO + GOLDEN_RATIO) * (timers.x % 100), 1);
		blueNoise.z = fmod(blueNoise.z + (GOLDEN_RATIO + GOLDEN_RATIO + GOLDEN_RATIO) * (timers.x % 100), 1);
		blueNoise.w = fmod(blueNoise.w + (GOLDEN_RATIO + GOLDEN_RATIO + GOLDEN_RATIO + GOLDEN_RATIO) * (timers.x % 100), 1);
	#endif
	
	const float2 blueNoiseRand[num] = {blueNoise.xy , blueNoise.zw , float2(-blueNoise.y, blueNoise.x) , float2(-blueNoise.w, blueNoise.z)};

	float sum = 0.0f;
	float acc = 0.0f;
	for (int k = 0; k < num; k++)
	{
		float dist = sqrt(blueNoiseRand[k].y); // Uniform
		
		if (0)
			dist = sqrt(-2.0f * log(max(1e-5f, blueNoiseRand[k].y))); // Gaussian
				
		float s, c;
		float a = blueNoiseRand[k].x;
		sincos(a * STALKER_TWO_PI, s, c);
		float2 rot = float2(c, s);

		float2 offset = rot * dist * scale;
		
		//offset *= (4.0 / STALKER_SCREEN_SIZE.xy); // Texel size
		
		//float weight = 1.0f; // box
		float weight = num - abs(k) + 1; // triangle
		//float weight = 1.0 / (1 + abs(k)); // pointy triangle
		
		acc += tex2Dproj(s_smap, float4(tc.xy + tc.w * offset, tc.zw)).x * weight;
		sum += weight;
	}
	acc = acc / sum;
	
	return	acc;
}

half shadow_jitter_sun(float4 tc, float2 tcJ)
{
	const int num = 4; 
	
	const float 	scale 	= (6.0f / float(SMAP_size));
	
	float4 blueNoise = tex2D(s_bluenoise, tcJ);
	
	#ifdef STALKER_USE_TAA
		blueNoise.x = fmod(blueNoise.x + (GOLDEN_RATIO) * (timers.x % 100), 1);
		blueNoise.y = fmod(blueNoise.y + (GOLDEN_RATIO + GOLDEN_RATIO) * (timers.x % 100), 1);
		blueNoise.z = fmod(blueNoise.z + (GOLDEN_RATIO + GOLDEN_RATIO + GOLDEN_RATIO) * (timers.x % 100), 1);
		blueNoise.w = fmod(blueNoise.w + (GOLDEN_RATIO + GOLDEN_RATIO + GOLDEN_RATIO + GOLDEN_RATIO) * (timers.x % 100), 1);
	#endif
	
	const float2 blueNoiseRand[num] = {blueNoise.xy , blueNoise.zw , float2(-blueNoise.y, blueNoise.x) , float2(-blueNoise.w, blueNoise.z)};

	float sum = 0.0f;
	float acc = 0.0f;
	for (int k = 0; k < num; k++)
	{
		float dist = sqrt(blueNoiseRand[k].y); // Uniform
		
		if (0)
			dist = sqrt(-2.0f * log(max(1e-5f, blueNoiseRand[k].y))); // Gaussian
				
		float s, c;
		float a = blueNoiseRand[k].x;
		sincos(a * STALKER_TWO_PI, s, c);
		float2 rot = float2(c, s);

		float2 offset = rot * dist * scale;
		
		//offset *= (4.0 / STALKER_SCREEN_SIZE.xy); // Texel size
		
		//float weight = 1.0f; // box
		float weight = num - abs(k) + 1; // triangle
		//float weight = 1.0 / (1 + abs(k)); // pointy triangle
		
		acc += tex2Dproj(s_smap, float4(tc.xy + tc.w * offset, tc.zw)).x * weight;
		sum += weight;
	}
	acc = acc / sum;
	
	return	acc;
}
#endif
