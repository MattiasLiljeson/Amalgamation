#include "deferredBaseVertexStruct.hlsl"
#include "deferredBasePixelStruct.hlsl"
#include "perFrameCBuffer.hlsl"

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