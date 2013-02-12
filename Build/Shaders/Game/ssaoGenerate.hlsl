#include "perFrameCBuffer.hlsl"
#include "utility.hlsl"

static const float blurFilter3[3][3] = {{0.01f,0.08f,0.01f},
									   {0.08f,0.64f,0.01f},
									   {0.01f,0.08f,0.01f}};

static const float blurFilter5[5][5] = {{0.01f,0.02f,0.04f,0.02f,0.01f},
										{0.02f,0.04f,0.08f,0.04f,0.02f},
										{0.04f,0.08f,0.16f,0.08f,0.04f},
										{0.02f,0.04f,0.08f,0.04f,0.02f},
										{0.01f,0.02f,0.04f,0.02f,0.01f}};

Texture2D gLightPass 		: register(t0);
Texture2D gNormalBuffer		: register(t1);
Texture2D depthBuffer		: register(t3);
Texture2D gRandomNormals 	: register(t2);

SamplerState pointSampler : register(s0);

cbuffer SSAO : register(b1)
{
	float scale;
	float bias;
	float intensity;
	float sampleRadius;
	float epsilon;
	const static float randSize = 64;
}

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

float3 getPosition(float2 uv)
{
	float depthValue = depthBuffer.Sample(pointSampler, uv).r;
	return getWorldPosFromTexCoord( uv, depthValue, gViewProjInverse);
}

float2 getRandomVector( float2 uv)
{
	return normalize(gRandomNormals.Sample(pointSampler, gRenderTargetSize*uv / randSize).xy * 2.0f - 1.0f);
}

float doAmbientOcclusion( float2 texCoordOrig, float2 uvOffset, float3 position, float3 normal)
{
	float3 diff = getPosition( texCoordOrig + uvOffset) - position;
	
	float3 v = normalize(diff);
	float d = length(diff)*scale;
	
	float ao = (max( 0.0, (dot(normal,v)-bias) * (1.0f/(1.0f+d)))*intensity)-epsilon;
	return clamp(ao,0.0f,0.5f);
}

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	vout.position = float4(p_input.position,1.0f);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

float4 PS(VertexOut input) : SV_TARGET
{
	float4 lightColor = float4(gLightPass.Sample(pointSampler,input.texCoord).rgb,1.0f);
	float depth = depthBuffer.Sample(pointSampler, input.texCoord).r;
	float4 randomNormals = float4(gRandomNormals.Sample(pointSampler, input.texCoord).rgb,1.0f);
	float4 sampleNormal = float4(gNormalBuffer.Sample(pointSampler, input.texCoord).rgb,1.0f);
	
	float3 position = getPosition(input.texCoord);
	float3 normal 	= convertSampledNormal(gNormalBuffer.Sample(pointSampler, input.texCoord).rgb);
	float2 rand 	= getRandomVector(input.texCoord);
	
	float ao = 0.0f;
	float radius = sampleRadius/depth;
	
	const float2 vec[4] = { float2 (1,0), float2 (-1,0),
							float2 (0,1), float2 (0,-1)};
	
	const int iterations = 4;
	for ( int i = 0; i < iterations; i++)
	{
		float2 coord1 = reflect(vec[i], rand)*radius;
		float2 coord2 = float2 (coord1.x*0.707f - coord1.y*0.707f,
								coord1.x*0.707f + coord1.y*0.707f);
								
		ao += doAmbientOcclusion(input.texCoord, coord1*0.25f, position, normal);
		ao += doAmbientOcclusion(input.texCoord, coord2*0.5f, position, normal);
		ao += doAmbientOcclusion(input.texCoord, coord1*0.75f, position, normal);
		ao += doAmbientOcclusion(input.texCoord, coord2, position, normal);
	}
	
	//ao/=(float)iterations*4.0f;
	
	ao = 1.0f - ao;
	//return float4(1,1,1,1);
	//return float4(0,0,0,0);
	//return float4(0.0f, 0.0f, 0.0f, position.x );
	//return float4(0.0f, 0.0f, 0.0f, lightColor.x );
	//return float4(0.0f, 0.0f, 0.0f, normal.x );
	return float4(0.0f, 0.0f, 0.0f, depth );
	return float4(0.0f, 0.0f, 0.0f, ao );
}