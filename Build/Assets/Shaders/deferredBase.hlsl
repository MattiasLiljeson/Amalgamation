cbuffer VertexProgramCBuffer
{
    float4 color;
	float4 pos;
};

struct VertexIn
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD; 
};
struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
};

struct PixelOut
{
	float4 diffuse	: SV_TARGET0;		//diffuse
	float4 normal	: SV_TARGET1;		//normal
};

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	vout.position = p_input.position;
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

PixelOut PS(VertexOut p_input)
{
	PixelOut pixelOut;
	pixelOut.diffuse = color;
	pixelOut.normal = float4(p_input.texCoord.x,p_input.texCoord.y,0.0f,1.0f);

	return pixelOut;
}

