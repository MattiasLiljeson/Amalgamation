#include "tesselationStruct.hlsl"
#include "deferredBaseVertexStruct.hlsl"
#include "perFrameCBuffer.hlsl"

TesselationStruct VS(VertexIn p_input)
{
	TesselationStruct vout;
	
	vout.position = mul(float4(p_input.position,1.0f), p_input.instanceTransform).xyz;
	vout.normal = mul(float4(p_input.normal,0.0f), p_input.instanceTransform).xyz;
	vout.tangent = mul(float4(p_input.tangent,0.0f),p_input.instanceTransform).xyz;
	vout.texCoord = p_input.texCoord;
    vout.gradientColor = p_input.gradientColor;
	vout.flags = p_input.flags;
	vout.colorOverlay = p_input.colorOverlay;
	
	return vout;
}