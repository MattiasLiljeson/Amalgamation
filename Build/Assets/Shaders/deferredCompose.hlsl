#include "lightLib.hlsl"

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecular : register(t2);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD; 
};
struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	vout.position = float4(p_input.position,1.0f);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

float4 PS(VertexOut p_input) : SV_TARGET
{
	float4 diffuseColor = float4(gDiffuseMap.Sample(pointSampler, p_input.texCoord));
	float4 normalColor = float4(gNormalMap.Sample(pointSampler, p_input.texCoord));	
	float4 specular = float4(gSpecular.Sample(pointSampler, p_input.texCoord));
	
	return specular;
	// Normal in -1 to 1 range
	float4 normal = normalColor*2.0f-1.0f;
	
	// temporary hard coded lighting test
	Light temp;
	temp.diffusePower = 0.3f;
	temp.specularPower = 0.0f;
	temp.vec = float4(0.0f,-1.0f,0.0f,0.0f); // w==0, directional light
	temp.diffuseColor = float4(1.0f,1.0f,1.0f,0.0f);
	temp.specularColor = float4(0.0f,0.0f,0.0f,0.0f);
	SurfaceLightingData lightManip = Lambert(temp, normal);
	
	// smash'em together
	return diffuseColor+lightManip.diffuseColor.r;
}

