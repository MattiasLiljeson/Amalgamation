#include "perFrameCBuffer.hlsl"
#include "utility.hlsl"
#include "lightLib.hlsl"

Texture2D gNormalMap 		: register(t1);
Texture2D gDepth 			: register(t3);
//Texture2D gRandomNormals	: register(t4);

SamplerState pointSampler : register(s0);
SamplerState shadowSampler : register(s1);

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

float3 getPosition( float2 uv, float depth )
{
	//float depthValue = gDepth.Sample(shadowSampler, uv).r;
	return getWorldPosFromTexCoord( uv, depth, gViewProjInverse);
}

// Input: It uses texture coords as the random number seed.
// Output: Random number: [0,1), that is between 0.0 and 0.999999... inclusive.
// Author: Michael Pohoreski
// Copyright: Copyleft 2012 :-)
float random( float2 p )
{
  // We need irrationals for pseudo randomness.
  // Most (all?) known transcendental numbers will (generally) work.
  const float2 r = float2(
    23.1406926327792690,  // e^pi (Gelfond's constant)
     2.6651441426902251); // 2^sqrt(2) (Gelfond–Schneider constant)
  return frac( cos( fmod( 123456789., 1e-7 + 256. * dot(p,r) ) ) );  
}

float2 getRandomVector( float2 uv )
{
	//return normalize(gRandomNormals.Sample(pointSampler, gRenderTargetSize*uv / randSize).xy * 2.0f - 1.0f);

	float2 rand;
	rand.x = random( uv.xy );
	rand.y = random( uv.yx );
	rand = normalize( rand );
	return rand;
}

float doAmbientOcclusion( float2 texCoordOrig, float2 uvOffset, float3 position, float3 normal)
{
	float depthValue = gDepth.Sample(shadowSampler, texCoordOrig).r;
	float3 diff = getPosition( texCoordOrig + uvOffset, depthValue ) - position;
	
	float3 v = normalize(diff);
	float d = length(diff)*scale;
	
	float ao = (max( 0.0, (dot(normal,v)-bias) * (1.0f/(1.0f+d)))*intensity)-epsilon;
	return clamp(ao,0.0f,0.5f);
}

float computeCoc(float depth)
{
	float limNear = 40.0f, limFar = 100.0f, coc =0.0f;

	if( depth > limFar ) {
		coc = (depth-limFar)/(gFarPlane-limFar);
	} else if( depth < limNear ) {
		coc = 1.0f - (depth/limNear);
	}
	coc *= coc; // Quad falloff
	return coc;
}

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	vout.position = float4(p_input.position,1.0f);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

PixelOut PS(VertexOut input)
{
	float depth = gDepth.Sample(pointSampler, input.texCoord).r;
	float3 position = getPosition(input.texCoord, depth);
	float pixelDepthW = length(position-gCameraPos.xyz);
	float3 normal 	= convertSampledNormal(gNormalMap.Sample(pointSampler, input.texCoord).rgb);
	float2 rand 	= getRandomVector( position.xy );

	// Generate ao
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
	ao = 1.0f - ao;

	// Calc Circle of Cunfusion for Dof
	float coc = computeCoc(pixelDepthW);

	PixelOut pout;
	pout.lightDiffuse = float4( 0, 0, 0, ao );
	pout.lightSpecular = float4( 0, 0, 0, coc );
	return pout;
	//return float4(0.0f, 0.0f, 0.0f, ao );
}