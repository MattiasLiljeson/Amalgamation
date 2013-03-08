#include "perFrameCBuffer.hlsl"
#include "utility.hlsl"

static const float g_LIGHT_MULT = 10;

static const float blurFilter3[3][3] = {{0.01f,0.08f,0.01f},
									   {0.08f,0.64f,0.01f},
									   {0.01f,0.08f,0.01f}};

static const float blurFilter5[5][5] = {{0.01f,0.02f,0.04f,0.02f,0.01f},
										{0.02f,0.04f,0.08f,0.04f,0.02f},
										{0.04f,0.08f,0.16f,0.08f,0.04f},
										{0.02f,0.04f,0.08f,0.04f,0.02f},
										{0.01f,0.02f,0.04f,0.02f,0.01f}};

#define NUM_TAPS 12
static const float2 poisson[NUM_TAPS] = {
	{-.326,-.406},
	{-.840,-.074},
	{-.696, .457},
	{-.203, .621},
	{ .962,-.195},
	{ .473,-.480},
	{ .519, .767},
	{ .185,-.893},
	{ .507, .064},
	{ .896, .412},
	{-.322,-.933},
	{-.792,-.598}
};

Texture2D g_diffuse				: register(t0);
Texture2D g_normal				: register(t1);
Texture2D g_specular			: register(t2);
Texture2D g_diffLight 			: register(t3);
Texture2D g_specLight			: register(t4);

Texture2D g_diffuseLowRes		: register(t5);
Texture2D g_normalLowRes		: register(t6);
Texture2D g_specularLowRes		: register(t7);
Texture2D g_specLightLowRes 	: register(t8);
Texture2D g_diffLightLowRes		: register(t9);

Texture2D g_depth				: register(t10);

SamplerState g_samplerPointWrap : register(s0);

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

float3 getPosition(float2 p_uv,float p_depth) 
{
	return getWorldPosFromTexCoord( p_uv, p_depth, gViewProjInverse);
}

// Depth of field blur
float4 PoissonDOF( float2 texCoord, uint3 index )
{
	float maxCoCRadius=5.0f, maxCoCDiameter = maxCoCRadius*2;
	float radiusScale=1.0f/g_lowResDivider; // radius of CoC on low res downsampled image
	//float radiusScale=1.0f; // radius of CoC on low res downsampled image

	float4 outColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	float discRadius, discRadiusLow, centerCoc;

	// Convert depth of pixel to blur radius(radius of the poisson disc)
	centerCoc = g_specLight.Load(index).a;
	discRadius = abs(centerCoc * maxCoCDiameter);
	discRadiusLow = discRadius*radiusScale;

	// Step size
	float2 gDX_Tex = float2(1/gRenderTargetSize.x, 1/gRenderTargetSize.y);
	float2 gDX_TexDOF = gDX_Tex/g_lowResDivider;
	for (int i=0; i < NUM_TAPS; i++)
	{
		// Get the tex-coords for high- and low-res tap
		float2 coordLow = texCoord + (gDX_TexDOF * poisson[i] * discRadiusLow);
		float2 coordHigh = texCoord + (gDX_Tex * poisson[i] * discRadius);
			
		float4 diffBuffLow  	= g_diffuseLowRes.Sample( g_samplerPointWrap, coordLow );
		float4 diffLightLow  	= g_specLightLowRes.Sample( g_samplerPointWrap, coordLow );
		float4 diffLow = diffBuffLow * diffLightLow * g_LIGHT_MULT;

		float4 specBuffLow  	= g_specularLowRes.Sample( g_samplerPointWrap, coordLow );
		float4 specLightLow  	= g_diffLightLowRes.Sample( g_samplerPointWrap, coordLow );
		float4 specLow = specBuffLow * specLightLow * g_LIGHT_MULT;
		float4 finalLow 		= diffLow + specLow;

		float4 diffBuffHigh  	= g_diffuse.Sample( g_samplerPointWrap, coordHigh );
		float4 diffLightHigh	= g_diffLight.Sample( g_samplerPointWrap, coordHigh );
		float4 diffHigh = diffBuffHigh * diffLightHigh * g_LIGHT_MULT;

		float4 specBuffHigh  	= g_specular.Sample( g_samplerPointWrap, coordHigh );
		float4 specLightHigh  	= g_specLight.Sample( g_samplerPointWrap, coordHigh );
		float4 specHigh = specBuffHigh * specLightHigh * g_LIGHT_MULT;
		float4 finalHigh = diffHigh + specHigh;

		float coc = g_specLight.Sample( g_samplerPointWrap, coordHigh ).a;
		// Mix low- and high-res taps based on blurriness
		float4 tap = lerp( finalHigh, finalLow, coc );

		// Ignore taps that are closer than the center and in focus
		tap.a = (tap.a >= centerCoc) ? 1.0f : abs(tap.a * 2.0f - 1.0f);

		outColor.rgb += tap.rgb * tap.a;
		outColor.a += tap.a;
	}

	return outColor/outColor.a;
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

	float depth = g_depth.Load( index ).r;

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
	float coc = 0.0f;

	[unroll]
	for( int x=-2; x<3; x++ ) {
		[unroll]
		for( int y=-2; y<3; y++ ) {
			uint3 idx = index+uint3(x,y,0);
			float blurFactor = blurFilter5[x+2][y+2];

			finalAO += g_diffLight.Load( idx ).a * blurFactor;
			coc += g_specLight.Load( idx ).a * blurFactor;
			
			sampledGlow = g_diffuse.Load( index+uint3(x*2,y*2,0) ).rgba;
			sampledGlow.rgb *= sampledGlow.a;
			finalEmissiveValue += sampledGlow.rgb * blurFactor;
			
			float lightBloom = 1.8f;
			uint3 lightBloomIdx = index+uint3(x,y,0)*lightBloom;
			
			// some bloom from highlights
			float3 light = 10.0f * ( g_specular.Load( lightBloomIdx ) * g_specLight.Load( lightBloomIdx ));
			light += 10.0f * (g_diffuse.Load( lightBloomIdx ) * g_diffLight.Load( lightBloomIdx ));
			float str = max( 0.0f, length(light)-1.0f );

			float glowBloom = 1.8f;
			uint3 glowBloomIdx = index+uint3(x,y,0)*glowBloom;

			// sample glow and add blurred highlights as well
			sampledGlow = g_diffuse.Load( glowBloomIdx ).rgba + float4( light, str );
			sampledGlow.rgb *= sampledGlow.a * 2.0f;
			finalEmissiveValue += sampledGlow.rgb * blurFilter5[x+2][y+2];
		}
	}
	//return float4( coc, coc, coc, 1);
	float4 finalCol = PoissonDOF( input.texCoord, index );

	// apply ao
	finalCol *= float4( finalAO, finalAO, finalAO, 1.0f );
	finalCol += float4 (ambient,0.0f );	
	// apply fog
	finalCol = float4( lerp( finalCol.rgb, fog/*+(lightSpec+lightDiff)*0.01f*/, fogDepth), finalCol.a ); 
	// apply glow
	finalCol += float4( finalEmissiveValue, 0.0f );
	
	return float4( finalCol.rgb, 1.0f );
}