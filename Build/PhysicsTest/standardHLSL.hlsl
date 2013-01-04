cbuffer ConstBuffer
{
    matrix World;
    matrix View;
    matrix Proj;
	float  Scale;
};

struct VOut
{
    float4 position : SV_POSITION;
	float3 normal	: NORMAL;
};

VOut VShader(float3 position : POSITION, float3 normal : NORMAL, float2 texCoord : TEXCOORD)
{
    VOut output;
	output.position = mul(float4(position*Scale, 1.0f), World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Proj);
    output.normal 	= mul(float4(normal, 0.0f), World).xyz;

    return output;
}

float4 PShader(float4 position : SV_POSITION, float3 normal : NORMAL) : SV_TARGET
{
	float ilum = dot(normal, float3(0.707f, 0.707f, 0));
    return float4(1, 0, 0, 1) * ilum;
}