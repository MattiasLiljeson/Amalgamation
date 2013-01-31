#include "lightLib.hlsl"

Texture2D gLightPass 	: register(t0);

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
	float4 lightColor = float4(gLightPass.Sample(pointSampler,p_input.texCoord).rgb,1.0f);
	
	return lightColor;
}

