#include "perFrameCBuffer.hlsl"

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D glowTexture : register(t3);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 positionL : POSITION;	
	float3 normalL : NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangentL : TANGENT;	
	float3 binormalL : BINORMAL;
	float4x4 instanceTransform : INSTANCETRANSFORM;
};
struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
	float3 normal : NORMAL;
};

struct PixelOut
{
	float4 diffuse	: SV_TARGET0;		//diffuse
	float4 normal	: SV_TARGET1;		//normal
	float4 specular : SV_TARGET2;		//specular
};

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;

	float4x4 wvp = mul(p_input.instanceTransform,gViewProj);
	
	vout.position = mul(float4(p_input.position,1.0f), wvp);
	vout.normal = mul(float4(p_input.normal,0.0f), p_input.instanceTransform).xyz;
	vout.normal = normalize(vout.normal);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

PixelOut PS(VertexOut p_input)
{
	PixelOut pixelOut;
	pixelOut.diffuse = diffuseTexture.Sample(pointSampler, p_input.texCoord);
	pixelOut.normal = float4(p_input.normal,0.0f);
	pixelOut.specular = diffuseTexture.Sample(pointSampler, p_input.texCoord);

	return pixelOut;
}

