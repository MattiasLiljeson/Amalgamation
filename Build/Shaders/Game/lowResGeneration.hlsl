#include "common.hlsl"

struct VertexIn
{
	float3 position 		: POSITION;
	float2 texCoord 		: TEXCOORD; 
};
struct VertexOut
{
    float4 position			: SV_POSITION;
	float2 texCoord			: TEXCOORD;
};
struct PixelOut
{
	float4 diffuse 			: SV_TARGET0;
	float4 normal 			: SV_TARGET1;
	float4 specular 		: SV_TARGET2;
	float4 lightDiffus 		: SV_TARGET3;
	float4 lightSpecula 	: SV_TARGET4;
};

VertexOut VS(VertexIn input)
{
	VertexOut vout;
	vout.position = float4( input.position, 1.0f );
	vout.texCoord = input.texCoord;
    
	return vout;
}

PixelOut PS(VertexOut input)
{
	PixelOut pout;
	
	SamplerState samplerState = g_samplerAnisotropicClamp;
	pout.diffuse 		= g_diffuse.Sample( samplerState, input.texCoord );
	pout.normal 		= g_normal.Sample( samplerState, input.texCoord );
	pout.specular 		= g_specular.Sample( samplerState, input.texCoord );
	pout.lightDiffus 	= g_diffLight.Sample( samplerState, input.texCoord );
	pout.lightSpecula 	= g_specLight.Sample( samplerState, input.texCoord );

	return pout;
}