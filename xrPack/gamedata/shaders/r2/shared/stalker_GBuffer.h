#ifndef STALKER_GBUFFER_H
#define STALKER_GBUFFER_H

struct GBufferData
{
	float	materialID;
	float	occlusion;
	float	smoothness;
	float	depth;
	
	float3	position;
	float3	diffuseColor;
	float3	specularColor;
	float3	normal;

};

void DataToGBuffer(GBufferData data, out float4 outGBuffer0, out float4 outGBuffer1, out float4 outGBuffer2, out float4 outGBuffer3)
{
	outGBuffer0 = float4(data.position, data.depth);

	outGBuffer1 = float4(data.normal, data.materialID);

	outGBuffer2 = float4(data.diffuseColor, data.occlusion);
	
	outGBuffer3 = float4(data.specularColor, data.smoothness);
}

GBufferData DataFromGBuffer(float4 inGBuffer0, float4 inGBuffer1, float4 inGBuffer2, float4 inGBuffer3)
{
	GBufferData data = (GBufferData) 0;
		
	data.depth						= inGBuffer0.a;
	data.materialID					= inGBuffer1.a;
	data.occlusion					= inGBuffer2.a;
	data.smoothness					= inGBuffer3.a;

	data.position					= inGBuffer0.rgb;		
	data.normal						= inGBuffer1.rgb;
	data.diffuseColor				= accurateSRGBToLinear(inGBuffer2.rgb);
	data.specularColor				= accurateSRGBToLinear(inGBuffer3.rgb);


	return data;
}


#endif