Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;
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
	vout.position = float4(p_input.position,1.0f);
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

float4 PS(VertexOut p_input) : SV_TARGET
{
	float4 diffuseColor = float4(gDiffuseMap.Sample(pointSampler, p_input.texCoord));
	float4 normalColor = float4(gNormalMap.Sample(pointSampler, p_input.texCoord));
	// smash'em together
	return diffuseColor;
}

