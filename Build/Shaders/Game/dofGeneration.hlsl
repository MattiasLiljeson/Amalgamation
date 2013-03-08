Texture2D g_diffuse				: register(t0);
Texture2D g_normal				: register(t1);
Texture2D g_specular			: register(t2);
Texture2D g_diffLight 			: register(t3);
Texture2D g_specLight			: register(t4);
//Texture2D g_depth				: register(t10);

SamplerState g_samplerPointWrap	: register(s0);

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
	
	pout.diffuse 		= g_diffuse.Sample( g_samplerPointWrap, input.texCoord );
	pout.normal 		= g_normal.Sample( g_samplerPointWrap, input.texCoord );
	pout.specular 		= g_specular.Sample( g_samplerPointWrap, input.texCoord );
	pout.lightDiffus 	= g_diffLight.Sample( g_samplerPointWrap, input.texCoord );
	pout.lightSpecula 	= g_specLight.Sample( g_samplerPointWrap, input.texCoord );

	return pout;
}