cbuffer VertexProgramCBuffer
{
    float4 color;
	float4x4 vp;
};

Texture2D diffuseTexture : register(t0);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD; 
	float3 normal : NORMAL;
};
struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
	float3 normal : NORMAL;
};

struct PixelOut
{
	float4 diffuse	: SV_TARGET0;		//diffuse
	float4 normal	: SV_TARGET1;		//normal
};

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;

	vout.position = mul(float4(p_input.position,1.0f), vp);
	vout.normal = mul(float4(p_input.normal,0.0f), vp).xyz;

	vout.texCoord = p_input.texCoord;
    
	return vout;
}

PixelOut PS(VertexOut p_input)
{
	PixelOut pixelOut;
	pixelOut.diffuse = color * diffuseTexture.Sample(pointSampler, p_input.texCoord);
	pixelOut.normal = float4(p_input.normal,0.0f);

	return pixelOut;
}

