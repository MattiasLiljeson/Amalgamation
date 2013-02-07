#include "perFrameCBuffer.hlsl"
#include "normalMapping.hlsl"

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D specularTexture : register(t2);
Texture2D glowTexture : register(t3);

SamplerState pointSampler : register(s0);

struct VertexIn
{
	float3 position : POSITION;	
	float3 normal 	: NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent 	: TANGENT;	
	float3 binormal : BINORMAL;
	float4x4 instanceTransform : INSTANCETRANSFORM;
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

VertexOut VS(VertexIn p_input)
{
	VertexOut vout;

	float4x4 wvp = mul(p_input.instanceTransform,gViewProj);
	
	vout.position = mul(float4(p_input.position,1.0f), wvp);
	vout.normal = mul(float4(p_input.normal,0.0f), p_input.instanceTransform).xyz;
	vout.tangent = mul(float4(p_input.tangent,0.0f),p_input.instanceTransform).xyz;
	vout.texCoord = p_input.texCoord;
    
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

