cbuffer HullData
{
	float4  Amount;
};

struct VOut
{
    float3 wpos         : POSITION;
	float3 normal	    : NORMAL;
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
	float2 texCoord     : TEXCOORD;
};

//////HULL SHADER////////////
struct COut
{
	float edges[3] 	: SV_TessFactor;
	float inside 	: SV_InsideTessFactor;
};
struct HOut
{
    float3 wpos         : POSITION;
	float3 normal	    : NORMAL;
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
	float2 texCoord     : TEXCOORD;
};

COut PatchConstant(InputPatch<VOut, 3> input, uint patchID: SV_PrimitiveID)
{
	COut output;
	
	//The amount of points added on each respective edge of the triangle
	output.edges[0] = Amount.x;
	output.edges[1] = Amount.y;
	output.edges[2] = Amount.z;
	
	//The amount of points added INSIDE the triangle
	output.inside = Amount.w;
	
	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstant")]
HOut HShader(InputPatch<VOut, 3> patch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	HOut output;
	output.wpos 	= patch[pointID].wpos;
	output.normal 	= patch[pointID].normal;
	output.tangent 	= patch[pointID].tangent;
	output.bitangent = patch[pointID].bitangent;
	output.texCoord 	= patch[pointID].texCoord;
	return output;
}