cbuffer VertexProgramCBuffer
{
    float4 color;
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

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;
	vout.position = p_input.position;
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

