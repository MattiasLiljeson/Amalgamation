#include "perFrameCBuffer.hlsl"

// Total of 120 bytes
struct VertexIn
{
	float3 position : POSITION;	//12 bytes
	float3 normal 	: NORMAL;	//12 bytes
	float2 texCoord : TEXCOORD; //8 bytes
	float3 tangent 	: TANGENT;	//12 bytes
	float3 binormal : BINORMAL;	//12 bytes
	float4x4 instanceTransform : INSTANCETRANSFORM;	//64 bytes
};

cbuffer perShadow : register(b1)
{
	float activeShadow;
};

float4 VS(VertexIn p_input) : SV_POSITION
{
	float4x4 wvp 	= mul(p_input.instanceTransform, shadowViewProj[activeShadow]);	
	return mul( float4 (p_input.position,1.0f),wvp);
}