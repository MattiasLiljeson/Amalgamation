#include "perFrameCBuffer.hlsl"
#include "deferredBasePixelStruct.hlsl"
#include "animationVertexStruct.hlsl"

Texture1D boneTexture : register(t0);

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

PixelIn VS(VertexIn p_input)
{
	int id = p_input.instanceId;
	float4 pos = float4(p_input.position, 1.0f);
	
	float4 norm = float4(p_input.normal, 0.0f);
    float4 tan = float4(p_input.tangent, 0.0f);
	float4 bin = float4(p_input.binormal, 0.0f);

	float3 p1 = float3(0.0f, 0.0f, 0.0f);
	float4 n = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 t = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 b = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    [unroll]
	for (int i = 0; i < 4; i++)
	{
		int index = p_input.indices1[i] + p_input.flags[1]*p_input.instanceId;
		float4x4 bonemat = getMatrix(index);
	
		p1 += (p_input.weights1[i] * mul(pos, bonemat)).xyz;
		n += p_input.weights1[i] * mul(norm, bonemat);
        t += p_input.weights1[i] * mul(tan, bonemat);
		b += p_input.weights1[i] * mul(bin, bonemat);
	}
	PixelIn vout;
	float4x4 wvp = mul(p_input.instanceTransform,gViewProj);

	vout.position = mul(float4(p1,1.0f), wvp);
	vout.normal = mul(n, p_input.instanceTransform).xyz;
	vout.tangent = mul(t,p_input.instanceTransform).xyz;
	vout.texCoord = p_input.texCoord;
	vout.gradientColor = p_input.gradientColor;
	vout.flags = p_input.flags;
	
	return vout;
}