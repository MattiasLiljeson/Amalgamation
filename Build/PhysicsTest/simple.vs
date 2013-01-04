cbuffer MatrixBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Proj;
};

struct VIn
{
	float3 position : POSITION; 
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct VOut
{

    float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float4 color 	: COLOR;
};

VOut VShader(VIn pIn)
{
    VOut output;
	output.position = mul(float4(pIn.position, 1.0f), World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Proj);
	output.normal = mul(float4(pIn.normal, 0.0f), World).xyz;
    output.color = pIn.color;

    return output;
}