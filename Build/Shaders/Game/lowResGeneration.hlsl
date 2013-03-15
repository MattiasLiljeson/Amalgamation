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

	pout.diffuse 		= float4( 0.0f, 0.0f, 0.0f, 0.0f);
	pout.normal 		= float4( 0.0f, 0.0f, 0.0f, 0.0f);
	pout.specular 		= float4( 0.0f, 0.0f, 0.0f, 0.0f);
	pout.lightDiffus 	= float4( 0.0f, 0.0f, 0.0f, 0.0f);
	pout.lightSpecula 	= float4( 0.0f, 0.0f, 0.0f, 0.0f);
	
	SamplerState samplerState = g_samplerAnisotropicClamp;

	[unroll]
	for( int x=-2; x<3; x++ ) {
		[unroll]
		for( int y=-2; y<3; y++ )
		{
			// Use 2 instead of g_LOW_RES_MULT, as the offset limits are hit when using g_LOW_RES_MULT.
			int2 idx = int2( x*2, y*2 );
			float blurFactor = blurFilter5[x+2][y+2];

			pout.diffuse 		+= blurFactor * g_diffuse.Sample( samplerState, input.texCoord, idx.xy );
			pout.normal 		+= blurFactor * g_normal.Sample( samplerState, input.texCoord, idx.xy );
			pout.specular 		+= blurFactor * g_specular.Sample( samplerState, input.texCoord, idx.xy );
			pout.lightDiffus 	+= blurFactor * g_diffLight.Sample( samplerState, input.texCoord, idx.xy );
			pout.lightSpecula 	+= blurFactor * g_specLight.Sample( samplerState, input.texCoord, idx.xy );
		}
	}

	return pout;
}