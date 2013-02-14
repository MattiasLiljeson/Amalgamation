cbuffer ConstBuffer
{
    matrix World;
    matrix View;
    matrix Proj;
	matrix Palette[200];
	float  Scale;
	float TextureScale;
};

Texture1D boneTexture : register(t0);
SamplerState pointSampler : register(s0);

struct VIn
{
	float3 position : POSITION;
	float3 normal : NORMAL; 
	float2 texCoord : TEXCOORD; 
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT; 
	float4 weights1 : AWEIGHTS; 
	float4 weights2 : BWEIGHTS;
	int4 indices1 : AINDICES;
	int4 indices2 : BINDICES;
};
struct VOut
{
    float4 position     : SV_POSITION;
    float3 wpos         : POSITION;
	float3 normal	    : NORMAL;
    float3 tangent      : TANGENT;
    float3 bitangent    : BITANGENT;
	float2 texCoord     : TEXCOORD;
};

float4x4 getMatrix(int pIndex)
{
	pIndex = pIndex * 4;
	float4 row1 = boneTexture.Load(int2(pIndex, 0));
	float4 row2 = boneTexture.Load(int2(pIndex+1, 0));
	float4 row3 = boneTexture.Load(int2(pIndex+2, 0));
	float4 row4 = boneTexture.Load(int2(pIndex+3, 0));

	float4x4 bonemat = float4x4(row1, row2, row3, row4);
	return bonemat;
}

VOut VShader(VIn pInput)
{
	float4 pos = float4(pInput.position, 1.0f);
	
	float4 norm = float4(pInput.normal, 0.0f);
    float4 tan = float4(pInput.tangent, 0.0f);
	float4 bin = float4(pInput.bitangent, 0.0f);

	float3 p1 = float3(0.0f, 0.0f, 0.0f);
	float4 n = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 t = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 b = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    [unroll]
	for (int i = 0; i < 4; i++)
	{
		float4x4 bonemat = getMatrix(pInput.indices1[i]);
	
		p1 += (pInput.weights1[i] * mul(pos, bonemat)).xyz;
		n += pInput.weights1[i] * mul(norm, bonemat);
        t += pInput.weights1[i] * mul(tan, bonemat);
		b += pInput.weights1[i] * mul(bin, bonemat);
	}
    [unroll]
	for (int i = 0; i < 4; i++)
	{
		float4x4 bonemat = getMatrix(pInput.indices2[i]);
		
		p1 += (pInput.weights2[i] * mul(pos, bonemat)).xyz;
		n += pInput.weights2[i] * mul(norm, bonemat);
        t += pInput.weights2[i] * mul(tan, bonemat);
		b += pInput.weights2[i] * mul(bin, bonemat);
	}

	VOut vOut;
	vOut.position = mul(float4(p1 * Scale, 1.0f), World);
    vOut.wpos  = vOut.position.xyz;
    vOut.position = mul(vOut.position, View);
    vOut.position = mul(vOut.position, Proj);
	
    vOut.normal 	= mul(n, World).xyz;
	vOut.normal     = normalize(vOut.normal);
    vOut.tangent 	= mul(t, World).xyz;
	vOut.tangent    = normalize(vOut.tangent);
    vOut.bitangent 	= mul(b, World).xyz;
	vOut.bitangent  = normalize(vOut.bitangent);
	vOut.texCoord 	= pInput.texCoord * (1/TextureScale);
	return vOut;
}