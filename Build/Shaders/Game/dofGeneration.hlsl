Texture2D gDiffuseBuffer	: register(t0);
Texture2D gNormalBuffer		: register(t1);
Texture2D gSpecularBuffer	: register(t2);
Texture2D gLightDiffuse 	: register(t3);
Texture2D gLightSpecular	: register(t4);
//Texture2D gDepthBuffer		: register(t5);

SamplerState pointSampler	: register(s0);

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

//float4 PS(VertexOut input) : SV_TARGET
//{
PixelOut PS(VertexOut input)// : SV_TARGET
{

	PixelOut pout;// = {0};
	pout.diffuse = float4(1,0,0,1);
	//return pout.diffuse;
	//return pout;
	//pout.diffuse 		= gDiffuseBuffer.Sample( pointSampler, input.texCoord );
	pout.normal 		= gNormalBuffer.Sample( pointSampler, input.texCoord );
	pout.specular 		= gSpecularBuffer.Sample( pointSampler, input.texCoord );
	pout.lightDiffus 	= gLightDiffuse.Sample( pointSampler, input.texCoord );
	pout.lightSpecula 	= gLightSpecular.Sample( pointSampler, input.texCoord );
	//pout.diffuse = float4( 1.0f, 0.0f ,0.0f, 1.0f );
	//pout.normal = pout.specular = pout.lightDiffuse =
	//pout.lightSpecular

	return pout;
	//return pout.normal;
}