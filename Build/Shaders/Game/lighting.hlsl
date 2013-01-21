#include "lightLib.hlsl"
#include "utility.hlsl"
//#include "perFrameCBuffer.hlsl"

cbuffer VertexProgramCBuffer
{
	float4 color;
	float4x4 vp;
	float4x4 vpInv;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecular : register(t2);
Texture2D gDepth : register(t3);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;	
	float4x4 instanceTransform : INSTANCETRANSFORM;
};
struct VertexOut
{
	float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD0;
};

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	float4x4 wvp = mul(p_input.instanceTransform,vp);
	vout.position = mul( float4(p_input.position, 1.0f), wvp );
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
	float depth = gDepth.Load( index ); 

	float3 normalVec = convertSampledNormal( normalColor.xyz );
	float2 ndcPos = getNdcPos( p_input.position, float2( 1280.0f ,720.0f ) );
	float3 worldPos = getWorldPos( ndcPos, depth, vpInv );

	LightInfo light;
	light.pos = float3( 0.0f, 0.0f, 0.0f );
	light.type = 1; // Should be set by instance/mesh
	light.dir = float3( 1.0f, 0.0f, 0.0f ); // Only used by point lights
	light.range = 1000.0f;
	light.att = 0.01f;
	light.spotPower = 10.0f; //Not used;
	light.ambient = float4( 0.0f, 0.0f, 0.1f, 0.1f );
	light.diffuse = float4( .0f, .5f, .0f, 0.1f );
	light.spec = float4( .5f, .0f, .0f, .1f );
	light.enabled = true;

	SurfaceInfo surface;
	surface.diffuse = diffuseColor;
	surface.spec = specular;

	float3 eyePos = float3( 1.0f, 1.0f, 1.0f );

	float3	lightCol = pointLight( surface, light, eyePos, normalVec, worldPos );

	return float4( lightCol, 1.0f );
}

