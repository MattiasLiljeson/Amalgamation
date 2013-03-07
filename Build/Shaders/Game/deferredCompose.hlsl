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

Texture2D gDiffBuffer		: register(t0);
Texture2D gNormalBuffer		: register(t1);
Texture2D gSpecBuffer		: register(t2);
Texture2D gLightDiff 		: register(t3);
Texture2D gLightSpec		: register(t4);

Texture2D gDofDiffBuffer	: register(t5);
Texture2D gDofNormalBuffer	: register(t6);
Texture2D gDofSpecBuffer	: register(t7);
Texture2D gDofLightDiff 	: register(t8);
Texture2D gDofLightSpec		: register(t9);

Texture2D gDepthBuffer		: register(t10);

SamplerState pointSampler : register(s0);

cbuffer SSAO : register(b1)
{
	float scale;
	float bias;
	float intensity;
	float sampleRadius;
	float epsilon;
	float cocFactor;
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
	uint3 index;
	index.xy = input.position.xy;
	index.z = 0;

	float4 rawDiffColor = gDiffBuffer.Load( index );
	//float4 rawNormal 	= gNormalBuffer.Load( index );
	float4 rawSpecColor = gSpecBuffer.Load( index );
	float4 rawLightDiff = gLightDiff.Load( index ) * 10.0f;
	float4 rawLightSpec = gLightSpec.Load( index ) * 10.0f;

	float4 dofDiffColor = gDofDiffBuffer.Sample( pointSampler, input.texCoord );
	//float4 dofNormal 	= gDofNormalBuffer.Sample( pointSampler, input.texCoord );
	float4 dofSpecColor = gDofSpecBuffer.Sample( pointSampler, input.texCoord );
	float4 dofLightDiff = gDofLightDiff.Sample( pointSampler, input.texCoord ) * 10.0f;
	float4 dofLightSpec = gDofLightSpec.Sample( pointSampler, input.texCoord ) * 10.0f;

	float depth = gDepthBuffer.Load( index ).r;

	//float4 sampleNormal = float4(gNormalBuffer.Sample(pointSampler, input.texCoord).rgb,1.0f);
	float3 fog = gFogColorAndFogFar.rgb;
	float3 ambient = gAmbientColorAndFogNear.rgb;
	float2 fogNearFarPercentage = float2(gAmbientColorAndFogNear.a,gFogColorAndFogFar.a);
	float3 position = getPosition(input.texCoord,depth);

	// calc linear depths
	float pixelDepthW = length(position-gCameraPos.xyz);
	float linDepth = pixelDepthW / (gFarPlane-gNearPlane);
	fogNearFarPercentage.y=0.4f;
	float fogDepth = saturate(pixelDepthW / (gFarPlane*fogNearFarPercentage.y-gNearPlane));
	// saturate(pixelDepthW / (gFarPlane*fogNearFarPercentage.x-gNearPlane*(2.0f-fogNearFarPercentage.y)));
	

	float finalAO = 0.0f;
	float3 finalEmissiveValue = float3(0,0,0);	
	float4 sampledGlow;
	float2 scale = float2(1.0f/1280.0f, 1.0f/720.0f);
	//scale *= coc;
	[unroll]
	for( int x=-2; x<3; x++ )
	{
		[unroll]
		for( int y=-2; y<3; y++ )
		{
			//float2 offset = float2(x*scale.x, y*scale.y);
			//offset *= coc;
			//float2 idx = index.xy*scale + offset;
			uint3 idx = index+uint3(x,y,0);
			float blurFactor = blurFilter5[x+2][y+2];

			finalAO += gLightDiff.Load( idx ).a * blurFactor;
			
			sampledGlow = gDiffBuffer.Load( index+uint3(x*2,y*2,0) ).rgba;
			sampledGlow.rgb *= sampledGlow.a;
			finalEmissiveValue += sampledGlow.rgb * blurFactor;
		}
	}
	float coc = 0.0f;
	coc = dofLightSpec.a ;
	coc *= cocFactor;
	//return float4(coc,coc,coc,1);
	float4 dofDiff = dofDiffColor * dofLightDiff;
	return dofDiff;
	float4 dofSpec = dofSpecColor * dofLightSpec;
	float4 rawDiff = rawDiffColor * rawLightDiff;
	float4 rawSpec = rawSpecColor * rawLightSpec;
	float4 diff = coc*dofDiff + (1-coc)*rawDiff;
	//diff = rawDiff*0.5 + rawDiff*0.5;
	//diff = dofDiff;
	float4 spec = coc*dofSpec + (1-coc)*rawSpec;
	//spec = rawSpec;
	//spec = dofSpec;

	//float4 diffColor = coc*dofDiffColor + (1-coc)*rawDiffColor;
	//diffColor = dofDiffColor;
	//float4 specColor = coc*dofSpecColor + (1-coc)*rawSpecColor;
	//specColor = dofSpecColor;
	//float4 lightDiff = coc*dofLightDiff + (1-coc)*rawLightDiff;
	//lightDiff = dofLightDiff;
	//float4 lightSpec = coc*dofLightSpec + (1-coc)*rawLightSpec;
	//lightSpec = dofLightSpec;

	// add light
	float4 finalCol = float4(0,0,0,0);
	finalCol += diff + spec;
	//finalCol += specColor * lightSpec;
	//finalCol += diffColor * lightDiff;	
	// apply ao
	//finalCol *= float4( finalAO, finalAO, finalAO, 1.0f );
	//finalCol += float4 (ambient,0.0f );	
	// apply fog
	//finalCol = float4( lerp( finalCol.rgb, fog+(lightSpec+lightDiff)*0.01f, fogDepth), finalCol.a ); 
	// apply glow
	//finalCol += float4( finalEmissiveValue, 0.0f );
	
	return float4( finalCol.rgb, 1.0f );
}