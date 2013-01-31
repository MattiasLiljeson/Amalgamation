#include "perFrameCBuffer.hlsl"

struct VertexIn
{
	float3 position : POSITION;	
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent : TANGENT;	
	float3 binormal : BINORMAL;
	float4x4 instanceTransform : INSTANCETRANSFORM;
};

float4 VS(VertexIn p_input): SV_POSITION
{
	float4x4 wvp = p_input.instanceTransform * shadowViewProj;
	return mul( float4 (p_input.position,1.0f), wvp);
}