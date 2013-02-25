#include "deferredBasePixelStruct.hlsl"
#include "normalMapping.hlsl"

Texture2D diffuseTexture  : register(t0);
Texture2D normalTexture   : register(t1);
Texture2D specularTexture : register(t2);
Texture2D glowTexture     : register(t3);
Texture2D gradientTexture : register(t5);

SamplerState pointSampler : register(s0);

struct PixelOut
{
	float4 diffuse	: SV_TARGET0;		//diffuse
	float4 normal	: SV_TARGET1;		//normal
	float4 specular : SV_TARGET2;		//specular
};

PixelOut PS(PixelIn p_input)
{
	PixelOut pixelOut;
	
	int layerCount = p_input.flags.x+0.5f; //+0.5f is need for rounding errors
	float value = gradientTexture.Sample(pointSampler, p_input.texCoord).x;
	
	int index = value * layerCount;
	index = min(index, layerCount-1);
	pixelOut.diffuse = p_input.gradientColor[index];
	pixelOut.diffuse *= diffuseTexture.Sample(pointSampler, p_input.texCoord);
	pixelOut.diffuse.rgb *= p_input.colorOverlay.xyz;

	float3 normalT	= normalTexture.Sample(pointSampler, p_input.texCoord).xyz;
	pixelOut.normal = float4(calcWorldNormals(normalT, p_input.tangent, p_input.normal)*0.5f+0.5f,0.0f);

	pixelOut.specular.rgb = specularTexture.Sample(pointSampler, p_input.texCoord).rgb;
	pixelOut.diffuse.a = glowTexture.Sample(pointSampler, p_input.texCoord).r + p_input.colorOverlay.w;
	
	return pixelOut;
}