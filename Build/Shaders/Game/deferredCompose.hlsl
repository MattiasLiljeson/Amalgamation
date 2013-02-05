#include "perFrameCBuffer.hlsl"
#include "utility.hlsl"

Texture2D gLightPass : register(t0);
Texture2D shadowPass : register(t1);
Texture2D depthBuffer: register(t3);

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

float doShadowing(float4 positionHomo);

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
	float shadowTex 	= shadowPass.Sample(pointSampler, p_input.texCoord).r;
	float depth 		= depthBuffer.Sample(pointSampler, p_input.texCoord).r;
	
	//float2 ndcPos = getNdcPos( p_input.position.xy, gRenderTargetSize );
	//float3 worldPos = getWorldPos( ndcPos, depth, gViewProjInverse );
	
	return lightColor;
}