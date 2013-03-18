#include "perFrameCBuffer.hlsl"
#include "utility.hlsl"
#include "common.hlsl"

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

float3 getPosition( float2 p_uv,float p_depth ) 
{
	return getWorldPosFromTexCoord( p_uv, p_depth, gViewProjInverse);
}

// Depth of field blur
float4 PoissonDOF( float2 texCoord, uint3 index )
{
	float maxCoCRadius=5.0f, maxCoCDiameter = maxCoCRadius*2;
	float radiusScale=1.0f/g_LOW_RES_MULT; // radius of CoC on low res downsampled image
	//float radiusScale=1.0f; // radius of CoC on low res downsampled image

	float4 outColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	float discRadius, discRadiusLow, centerCoc;

	// Convert depth of pixel to blur radius(radius of the poisson disc)
	centerCoc = g_specLight.Load( index ).a;
	discRadius = abs( centerCoc * maxCoCDiameter );
	discRadiusLow = discRadius*radiusScale;

	// Step size
	float2 gDX_Tex = float2( 1/gRenderTargetSize.x, 1/gRenderTargetSize.y );
	float2 gDX_TexDOF = gDX_Tex/g_LOW_RES_MULT;
	for( int i=0; i<NUM_TAPS; i++ )
	{
		// Get the tex-coords for high- and low-res tap
			
		SamplerState lowSampler = g_samplerAnisotropicClamp;
		float2 coordLow = texCoord + (gDX_TexDOF * poisson[i] * discRadiusLow);
		float4 diffBuffLow  	= g_diffuseLowRes.Sample( lowSampler, coordLow );
		float4 diffLightLow  	= g_specLightLowRes.Sample( lowSampler, coordLow );
		float4 diffLow = diffBuffLow * diffLightLow * g_LIGHT_MULT;

		float4 specBuffLow  	= g_specularLowRes.Sample( lowSampler, coordLow );
		float4 specLightLow  	= g_diffLightLowRes.Sample( lowSampler, coordLow );
		float4 specLow = specBuffLow * specLightLow * g_LIGHT_MULT;
		float4 finalLow 		= diffLow + specLow;

		SamplerState highSampler = g_samplerPointClamp;
		float2 coordHigh = texCoord + (gDX_Tex * poisson[i] * discRadius);
		float4 diffBuffHigh  	= g_diffuse.Sample( highSampler, coordHigh );
		float4 diffLightHigh	= g_diffLight.Sample( highSampler, coordHigh );
		float4 diffHigh = diffBuffHigh * diffLightHigh * g_LIGHT_MULT;

		float4 specBuffHigh  	= g_specular.Sample( highSampler, coordHigh );
		float4 specLightHigh  	= g_specLight.Sample( highSampler, coordHigh );
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

VertexOut VS( VertexIn p_input )
{
	VertexOut vout;
	vout.position = float4(p_input.position,1.0f);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

float4 PS( VertexOut input ) : SV_TARGET
{
	uint3 index;
	index.xy = input.position.xy;
	index.z = 0;
	
	//float4 temp = g_diffuseLowRes.Load(index);
	//temp.w = 1.0f;
	//return temp;
	
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
	float sampledCoc;
	float sampledDepth;
	float2 scale = float2(1.0f/1280.0f, 1.0f/720.0f);
	float centerCoc = g_specLight.Load( index ).a;
	float coc = 0.0f;

	[unroll]
	for( int x=-2; x<3; x++ ) {
		[unroll]
		for( int y=-2; y<3; y++ ) {
			uint3 idx = index+uint3(x,y,0);
			float blurFactor = blurFilter5[x+2][y+2];

			finalAO += g_diffLight.Load( idx ).a * blurFactor;

			sampledCoc = g_specLight.Load( idx ).a;
			sampledDepth = g_depth.Load( idx ).r;
			//if( sampledDepth < depth ) {
				// If not protruding
				//coc +=  centerCoc * blurFactor;
				//coc += float4( 0.1, 0, 0, 1 );
			//} else {
				// If protruding
				coc += sampledCoc * blurFactor;
				//coc += float4( 0, 0, 0.1, 1 );
			//}
			
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
	//return float4( coc, coc, coc, 1 );
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