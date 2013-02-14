#include "perFrameCBuffer.hlsl"
#include "normalMapping.hlsl"


Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D glowTexture : register(t3);

Texture1D boneTexture : register(t0);


SamplerState pointSampler : register(s0);


struct VertexIn
{
	//Vertex Buffer 1
	float3 position : POSITION;	
	float3 normal 	: NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent 	: TANGENT;	
	float3 binormal : BINORMAL;
	
	//Vertex Buffer 2
	float4 weights1 : AWEIGHTS; 
	float4 weights2 : BWEIGHTS;
	int4 indices1 	: AINDICES;
	int4 indices2 	: BINDICES;
	
	//Vertex Buffer 3
	float4x4 instanceTransform : INSTANCETRANSFORM;
	uint instanceId : SV_InstanceID;

};
struct VertexOut
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
	float3 normal 	: NORMAL;
	float3 tangent 	: TANGENT;
};


struct PixelOut
{
	float4 diffuse	: SV_TARGET0;		//diffuse
	float4 normal	: SV_TARGET1;		//normal
	float4 specular : SV_TARGET2;		//specular
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

VertexOut VS(VertexIn p_input)
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
		float4x4 bonemat = getMatrix(p_input.indices1[i]);
	
		p1 += (p_input.weights1[i] * mul(pos, bonemat)).xyz;
		n += p_input.weights1[i] * mul(norm, bonemat);
        t += p_input.weights1[i] * mul(tan, bonemat);
		b += p_input.weights1[i] * mul(bin, bonemat);
	}


	VertexOut vout;


	float4x4 wvp = mul(p_input.instanceTransform,gViewProj);

	vout.position = mul(float4(p1,1.0f), wvp);
	vout.normal = mul(n, p_input.instanceTransform).xyz;
	vout.tangent = mul(t,p_input.instanceTransform).xyz;
	vout.texCoord = p_input.texCoord;
	
	/*vout.position = mul(float4(p_input.position,1.0f), wvp);
	vout.normal = mul(float4(p_input.normal,0.0f), p_input.instanceTransform).xyz;
	vout.tangent = mul(float4(p_input.tangent,0.0f),p_input.instanceTransform).xyz;
	vout.texCoord = p_input.texCoord;*/
    
	return vout;
}


PixelOut PS(VertexOut p_input)
{
	PixelOut pixelOut;
	pixelOut.diffuse = diffuseTexture.Sample(pointSampler, p_input.texCoord);


	// temp fog
	//float linDepth = pow(p_input.position.z, (gFarPlane-gNearPlane));
	//float4 fog = float4(linDepth,linDepth,linDepth,0.0f);


	//pixelOut.diffuse += fog;


	float3 normalT	= normalTexture.Sample(pointSampler, p_input.texCoord).xyz;
	pixelOut.normal = float4(calcWorldNormals(normalT, p_input.tangent, p_input.normal)*0.5f+0.5f,0.0f);


	pixelOut.specular = specularTexture.Sample(pointSampler, p_input.texCoord);


	return pixelOut;
}
