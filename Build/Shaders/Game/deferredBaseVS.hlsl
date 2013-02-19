#include "deferredBasePixelStruct.hlsl"
#include "perFrameCBuffer.hlsl"

struct VertexIn
{
	float3 position : POSITION;	
	float3 normal 	: NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent 	: TANGENT;	
	float3 binormal : BINORMAL;
	
	//Per instance
	float4x4 instanceTransform  : INSTANCETRANSFORM;
	float4x4 gradientColor 		: GRADIENTCOLOR;
	float4 flags 	: FLAGS;
};

PixelIn VS(VertexIn p_input)
{
	PixelIn vout;
	float4x4 wvp = mul(p_input.instanceTransform,gViewProj);
	
	vout.position = mul(float4(p_input.position,1.0f), wvp);
	vout.normal = mul(float4(p_input.normal,0.0f), p_input.instanceTransform).xyz;
	vout.tangent = mul(float4(p_input.tangent,0.0f),p_input.instanceTransform).xyz;
	vout.texCoord = p_input.texCoord;
	vout.gradientColor = p_input.gradientColor;
	vout.flags = p_input.flags;
    
	return vout;
}