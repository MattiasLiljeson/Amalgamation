cbuffer ConstBuffer
{
    matrix World;
    matrix View;
    matrix Proj;
	float  Scale;
};

struct VIn
{
	float3 position : POSITION;
	float3 normal : NORMAL; 
	float2 texCoord : TEXCOORD; 
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT; 
};
struct VOut
{
    float3 wpos         : POSITION;
	float3 normal	    : NORMAL;
};

VOut VShader(VIn pInput)
{
	VOut vOut;
	vOut.wpos = mul(float4(pInput.position * Scale, 1.0f), World);	
    vOut.normal 	= mul(float4(pInput.normal, 0), World).xyz;
	vOut.normal     = normalize(vOut.normal);
	return vOut;
}