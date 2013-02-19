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
Texture2D depthBuffer		: register(t2);
Texture2D gRandomNormals 	: register(t3);

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
	return gRandomNormals.Sample(pointSampler, gRenderTargetSize*uv / randSize).xy * 2.0f - 1.0f;
}

float doAmbientOcclusion( float2 texCoordOrig, float2 uvOffset, float3 position, float3 normal)
{
	float3 diff = getPosition( texCoordOrig + uvOffset) - position;
	
	float3 v = normalize(diff);
	float d = length(diff)*scale;
	
	return max( 0.0, (dot(normal,v)-bias) * (1.0f/(1.0f+d)))*intensity;
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
	float4 lightColor = gLightPass.Sample(pointSampler,input.texCoord);
	float depth = depthBuffer.Sample(pointSampler, input.texCoord).r;
	float4 randomNormals = float4(gRandomNormals.Sample(pointSampler, input.texCoord).rgb,1.0f);
	float4 sampleNormal = float4(gNormalBuffer.Sample(pointSampler, input.texCoord).rgb,1.0f);
	
	float3 position = getPosition(input.texCoord);
	float3 normal 	= convertSampledNormal(gNormalBuffer.Sample(pointSampler, input.texCoord).rgb);
	float2 rand 	= getRandomVector(input.texCoord);
	
	uint3 index;
	index.xy = input.position.xy;
	index.z = 0;
	float ao = 0.0f;

	//ao = lightColor.a;
	float aoMult = 1.0f;
	for(int x=-2;x<3;x++)
	{
		for(int y=-2;y<3;y++)
		{
			ao += gLightPass.Load( index+uint3(x,y,0) ).a * blurFilter5[x+2][y+2];
		}
	}

	//return float4( ao, ao, ao, 1.0f );
	//lightColor = float4(lightColor.r, lightColor.g, lightColor.b, 1.0f );
	lightColor = float4(lightColor.r*ao, lightColor.g*ao, lightColor.b*ao, 1.0f );
	float linDepth = length(position-gCameraPos) / ((1000.0f)-(300.0f));
	float4 fog = linDepth*float4(0.119f,0.296f,0.116f,0.0f);
	//float fogDepth = saturate(length(position-gCameraPos) / ((1200.0f)-(300.0f)));
	//float4 fog = float4(0.2f,0.0745f,0.0f,0.0f);
	//return lerp(lightColor,fog,saturate(fogDepth)); // can do this when light is separate from diffuse
	return lightColor+fog;
}