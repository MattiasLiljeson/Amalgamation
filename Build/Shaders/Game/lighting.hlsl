#include "perFrameCBuffer.hlsl"
#include "lightLib.hlsl"
#include "shadowing.hlsl"
#include "utility.hlsl"

Texture2D gDiffuseMap 	: register(t0);
Texture2D gNormalMap 	: register(t1);
Texture2D gSpecular 	: register(t2);
Texture2D gDepth 		: register(t3);

Texture2D gShadow1 		: register(t4);
Texture2D gShadow2 		: register(t5);
Texture2D gShadow3		: register(t6);
Texture2D gShadow4		: register(t7);

SamplerState pointSampler : register(s0);
SamplerState shadowSampler : register(s1);

//Total of 168 bytes
struct VertexIn
{
	//PerVertex
	float3 position 				: POSITION;					//12 bytes
	
	//PerInstance
	float4x4 instanceTransform 		: INSTANCETRANSFORM;		//64 bytes
	float3 	lightDir				: LIGHTDIR;					//12 bytes
	float 	range					: RANGE;					//4 bytes	
	float3 	attenuation				: ATTENUATION;				//12 bytes
	float 	spotLightConeSizeAsPow	: SPOTLIGHTCONESIZEASPOW;	//4 bytes
	float3 	color					: COLOR;					//16 bytes
	float 	lightEnergy				: LIGHTENERGY;
	int 	enabled 				: ENABLED;					//4 bytes
	int 	type 					: TYPE;						//4 bytes
	int		shadowIdx				: SHADOWIDX;				//4 bytes
};
struct VertexOut
{
	float4 position	: SV_POSITION;
	//float2 texCoord	: TEXCOORD0;
	LightInfo light 	: STRUCT;
};

VertexOut VS( VertexIn p_input )
{
	VertexOut vout;
	float4x4 wvp = mul(p_input.instanceTransform, gViewProj);
	vout.position = mul( float4(p_input.position, 1.0f), wvp );
	
	vout.light.pos						= float3( 
											p_input.instanceTransform[3][0],
											p_input.instanceTransform[3][1],
											p_input.instanceTransform[3][2] );
	vout.light.lightDir 				= p_input.lightDir; // Only used by point lights
	vout.light.range 					= p_input.range;
	vout.light.attenuation 				= p_input.attenuation;
	vout.light.spotLightConeSizeAsPow 	= p_input.spotLightConeSizeAsPow;
	vout.light.color 					= p_input.color;
	vout.light.lightEnergy 				= p_input.lightEnergy;
	vout.light.enabled 					= p_input.enabled;
	vout.light.type 					= p_input.type; // Should be set by instance/mesh
	vout.light.shadowIdx				= p_input.shadowIdx;
	
	return vout;
}

struct PixelOut
{
	float4 lightDiffuse 	: SV_TARGET0;
	float4 lightSpecular 	: SV_TARGET1;
};

PixelOut PS( VertexOut p_input ) : SV_TARGET
{
	uint3 index;
	index.xy = p_input.position.xy;
	index.z = 0;
	float4 normalColor	= gNormalMap.Load( index );	
	float depth = gDepth.Load( index ).x; 

	float3 normalVec = convertSampledNormal( normalColor.xyz );
	float2 ndcPos = getNdcPos( p_input.position.xy, gRenderTargetSize );
	float3 worldPos = getWorldPos( ndcPos, depth, gViewProjInverse );

	LightOut light;
	
	if( p_input.light.type == 0 ) { // Directional light
		light = parallelLight( p_input.light, gCameraPos.xyz, normalVec, worldPos );
	} else if ( p_input.light.type == 1 ) { // Point light
		light = pointLight( p_input.light, gCameraPos.xyz, normalVec, worldPos );
	} else if ( p_input.light.type == 2 ) { // Spot light
		light = spotLight( p_input.light, gCameraPos.xyz, normalVec, worldPos );
	}

	float shadowCoeff = 1.0f;
	int shadowIndex = p_input.light.shadowIdx;
	if( shadowIndex != -1 )
	{
		float4 shadowWorldPos = mul( float4(worldPos,1.0f), shadowViewProj[shadowIndex]);
		shadowCoeff = doShadowing(gShadow1, shadowSampler, shadowWorldPos);
	}
	//lightCol *= shadowCoeff;
	
	PixelOut pixelOut;
	pixelOut.lightDiffuse = float4( light.lightDiffuse, 0.0f ) * 0.1f;
	pixelOut.lightSpecular = float4( light.lightSpecular, 0.0f ) * 0.1f;
	return pixelOut;
}

