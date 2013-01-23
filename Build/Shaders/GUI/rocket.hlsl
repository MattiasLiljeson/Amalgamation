#include "../Game/perFrameCBuffer.hlsl"

Texture2D diffuseTexture : register(t0);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;	
	float3 normal 	: NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent 	: TANGENT;	
	float3 binormal : BINORMAL;
};

struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
	float4 color 	: COLOR;
};

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	vout.color.xyz 	= p_input.normal.xyz;
	vout.color.w 	= p_input.position.z;
	
	vout.position = mul(float4(p_input.position.xy,0.0f,1.0f), gViewProj);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

float4 PS(VertexOut p_input) : SV_TARGET
{
	float2 uv = p_input.texCoord;
	float4 diffuse = diffuseTexture.Sample( pointSampler, uv );
	float4 newColor = p_input.color.xyzw/255.0f;
	float4 finalColor = newColor*diffuse;
	
	return finalColor;
}

