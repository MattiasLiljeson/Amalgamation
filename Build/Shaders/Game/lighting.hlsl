#include "perFrameCBuffer.hlsl"
#include "lightLib.hlsl"
#include "utility.hlsl"

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecular : register(t2);
Texture2D gDepth : register(t3);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;	
	float4x4 instanceTransform : INSTANCETRANSFORM;
	//LightInfo light : LIGHT;
	
	float 	range		: RANGE;
	float3 	lightDir	: LIGHTDIR;
	float3 	attenuation	: ATTENUATION;
	float 	spotPower	: SPOTPOWER;
	float4 	ambient		: AMBIENT;
	float4 	diffuse		: DIFFUSE;
	float4 	specular	: SPECULAR;
	int 	enabled 	: ENABLED;
	int 	type 		: TYPE;
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
	
	vout.light.pos			= float3( 
								p_input.instanceTransform[3][0],
								p_input.instanceTransform[3][1],
								p_input.instanceTransform[3][2] );
	vout.light.type 		= p_input.type; //1; // Should be set by instance/mesh
	vout.light.lightDir 	= p_input.lightDir ;//float3( 1.0f, 0.0f, 0.0f ); // Only used by point lights
	vout.light.range 		= p_input.range; //1000.0f;
	vout.light.attenuation 	= p_input.attenuation; //0.01f;
	vout.light.spotPower 	= p_input.spotPower; //10.0f; //Not used;
	vout.light.ambient 		= p_input.ambient; //float4( 0.0f, 0.0f, 0.1f, 0.1f );
	vout.light.diffuse 		= p_input.diffuse; //float4( .0f, .5f, .0f, 0.1f );
	vout.light.specular 	= p_input.specular; //float4( .5f, .0f, .0f, .1f );
	vout.light.enabled 		= p_input.enabled; //true;
	
	return vout;
}

float4 PS( VertexOut p_input ) : SV_TARGET
{
	uint3 index;
	index.xy = p_input.position.xy;
	index.z = 0;
	float4 diffuseColor = float4( gDiffuseMap.Load( index ) );
	float4 normalColor	= float4( gNormalMap.Load( index ) );	
	float4 specular		= float4( gSpecular.Load( index ) );
	float depth = gDepth.Load( index ).x; 

	float3 normalVec = convertSampledNormal( normalColor.xyz );
	float2 ndcPos = getNdcPos( p_input.position.xy, gRenderTargetSize );
	float3 worldPos = getWorldPos( ndcPos, depth, gViewProjInverse );

	SurfaceInfo surface;
	surface.diffuse = diffuseColor;
	surface.specular = specular;

	float3	lightCol = pointLight( surface, p_input.light, gCameraPos, normalVec, worldPos, p_input.light.pos );

	return float4( lightCol, 1.0f );
}

