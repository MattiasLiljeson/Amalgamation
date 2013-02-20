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
Texture2D gDepthBuffer		: register(t2);
Texture2D gDiffuseBuffer	: register(t3);

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

// Changed by Jarl: removed additional depth sampling, added depth as param instead
float3 getPosition(float2 p_uv,float p_depth) 
{
	return getWorldPosFromTexCoord( p_uv, p_depth, gViewProjInverse);
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
	float4 diffuseColor = gDiffuseBuffer.Sample(pointSampler,input.texCoord);
	float4 lightColor = gLightPass.Sample(pointSampler,input.texCoord);
	float depth = gDepthBuffer.Sample(pointSampler, input.texCoord).r;
	//float4 randomNormals = float4(gRandomNormals.Sample(pointSampler, input.texCoord).rgb,1.0f);
	float4 sampleNormal = float4(gNormalBuffer.Sample(pointSampler, input.texCoord).rgb,1.0f);
	float3 fog = gFogColorAndFogFar.rgb;
	float3 ambient = gAmbientColorAndFogNear.rgb;
	float2 fogNearFarPercentage = float2(gAmbientColorAndFogNear.a,gFogColorAndFogFar.a);
	float3 position = getPosition(input.texCoord,depth);
	// calc linear depths
	float pixelDepthW = length(position-gCameraPos.xyz);
	float linDepth = pixelDepthW / (gFarPlane-gNearPlane);
	float fogDepth = saturate(pixelDepthW / ((gFarPlane*fogNearFarPercentage.x)-(gNearPlane*(2.0f-fogNearFarPercentage.y))));
	
	uint3 index;
	index.xy = input.position.xy;
	index.z = 0;
	float finalAO = 0.0f;

	float3 finalEmissiveValue = float3(0,0,0);
		
	float4 sampledColor;
	[unroll]
	for(int x=-2;x<3;x++)
	{
		[unroll]
		for(int y=-2;y<3;y++)
		{
			finalAO += gLightPass.Load( index+uint3(x,y,0) ).a * blurFilter5[x+2][y+2];
			
			sampledColor = gDiffuseBuffer.Load( index+uint3(x*2,y*2,0) ).rgba;
			sampledColor.rgb *= sampledColor.a;
			finalEmissiveValue += sampledColor.rgb * blurFilter5[x+2][y+2];
		}
	}
	// apply ao
	lightColor = float4(lightColor.r*finalAO, lightColor.g*finalAO, lightColor.b*finalAO, 1.0f );
	
	
	lightColor += float4(ambient,0.0f);
	lightColor = float4(lerp(lightColor.rgb,fog,saturate(fogDepth)),0.0f); // can do this when light is separate from diffuse
	return float4(lightColor.rgb+finalEmissiveValue.rgb,1.0f); // then all light can be added here like glow is now
}