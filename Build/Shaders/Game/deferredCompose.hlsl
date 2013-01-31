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
	
	float2 ndcPos = getNdcPos( p_input.position.xy, gRenderTargetSize );
	float3 worldPos = getWorldPos( ndcPos, depth, gViewProjInverse );
	// worldPos = GetWorldPosition( p_input.texCoord, depth);
	
	float4 shadowWorldPos = mul( float4(worldPos,1.0f), shadowViewProj);
	float shadowCoeff = doShadowing(shadowWorldPos);
	
	lightColor = lightColor*shadowCoeff;
	
	shadowTex = pow(shadowTex,100);
	//return float4(worldPos.rgb,1.0f);
	//return float4(shadowTex,shadowTex,shadowTex,1.0f);
	return lightColor;
}

float doShadowing(float4 positionHomo){
	float SHADOW_EPSILON = 0.01f;
	float SMAP_SIZE = 512.0f;
	
	positionHomo.xy /= positionHomo.w;
	
	float2 smTexCoord = float2(0.5f*positionHomo.x, -0.5f*positionHomo.y)+0.5f;
	
	if (smTexCoord.x < 0.0f || smTexCoord.x > 1.0f || smTexCoord.y < 0.0f || smTexCoord.y > 1.0f )
		return 1.0f;
	
	float depth = positionHomo.z/positionHomo.w;
	
	float dx = 1.0f/SMAP_SIZE;
	float s0 = (shadowPass.Sample(pointSampler, smTexCoord).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s1 = (shadowPass.Sample(pointSampler, smTexCoord + float2 (dx,0.0f)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s2 = (shadowPass.Sample(pointSampler, smTexCoord + float2 (0.0f,dx)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s3 = (shadowPass.Sample(pointSampler, smTexCoord + float2 (dx,dx)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	
	float2 texelPos = smTexCoord * SMAP_SIZE;
	float2 lerps = frac(texelPos);
	
	return float (lerp(lerp(s0,s1,lerps.x),lerp(s2,s3,lerps.x),lerps.y));
	//return s0;
}