cbuffer ConstBuffer
{
    matrix World;
    matrix View;
    matrix Proj;
	float  Scale;
	float TextureScale;
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
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
	float2 texCoord     : TEXCOORD;
};

VOut VShader(VIn pInput)
{
	VOut vOut;
	vOut.wpos = mul(float4(pInput.position * Scale, 1.0f), World);	
    vOut.normal 	= mul(float4(pInput.normal, 0), World).xyz;
	vOut.normal     = normalize(vOut.normal);
    vOut.tangent 	= mul(float4(pInput.tangent,0), World).xyz;
	vOut.tangent    = normalize(vOut.tangent);
    vOut.bitangent 	= mul(float4(pInput.bitangent, 0), World).xyz;
	vOut.bitangent  = normalize(vOut.bitangent);
	vOut.texCoord 	= pInput.texCoord * (1/TextureScale);
	return vOut;
}

/*VOut VShader(VIn pInput)
{
	float4 pos = float4(pInput.position, 1.0f);
	float4 norm = float4(pInput.normal, 0.0f);
    float4 tan = float4(pInput.tangent, 0.0f);
	float4 bin = float4(pInput.bitangent, 0.0f);

	float3 p1 = float3(0.0f, 0.0f, 0.0f);
	float4 n = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 t = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 b = float4(0.0f, 0.0f, 0.0f, 0.0f);

	matrix mat = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

		p1 += mul(pos, mat).xyz;
		n += mul(norm, mat);
        t +=  mul(tan, mat);
		b += mul(bin, mat);

	VOut vOut;
	vOut.wpos = mul(float4(p1 * Scale, 1.0f), World);
    vOut.normal 	= mul(n, World).xyz;
	vOut.normal     = normalize(vOut.normal);
    vOut.tangent 	= mul(t, World).xyz;
	vOut.tangent    = normalize(vOut.tangent);
    vOut.bitangent 	= mul(b, World).xyz;
	vOut.bitangent  = normalize(vOut.bitangent);
	vOut.texCoord = pInput.texCoord;
	return vOut;
}*/