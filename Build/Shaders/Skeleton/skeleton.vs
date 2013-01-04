cbuffer ConstBuffer
{
    matrix World;
    matrix View;
    matrix Proj;
	matrix Palette[200];
	float  Scale;
};

struct VIn
{
	uint index : INDEX;
};
struct VOut
{
    float4 position     : SV_POSITION;
};

VOut VShader(VIn pInput)
{
	float4 pos = float4(0, 0, 0, 1.0f);

	float3 p1 = mul(pos, Palette[pInput.index]).xyz;

	VOut vOut;
	vOut.position = mul(float4(p1 * Scale, 1.0f), World);
    vOut.position = mul(vOut.position, View);
    vOut.position = mul(vOut.position, Proj);
	return vOut;
}