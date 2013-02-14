//ADDED BY ANTON 5/2-13 HULL AND DOMAIN SHADER:
//		*struct patchConstantOut
//		*struct HullOut
//		*struct DomainOut
//		*PatchConstant function
//		*HS function
//		*DS function
//ALSO CHANGED THE FOLLOWING IN EXISTING CODE:
//		*Changed VertexOut -     float4 position	: SV_POSITION; -> float3 position	: POSITION;

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
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent : TANGENT;	
	float3 binormal : BINORMAL;
	float4x4 instanceTransform : INSTANCETRANSFORM;
};
struct VertexOut
{
    float3 position	: POSITION;
	float2 texCoord	: TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

//Output from the patch constant function 
struct patchConstantOut
{
	float edges[3] 	: SV_TessFactor;
	float inside 	: SV_InsideTessFactor;
};

//Output from the hull shader
struct HullOut
{
    float3 position     : POSITION;
	float3 normal	    : NORMAL;
    float3 tangent      : TANGENT;
	float2 texCoord     : TEXCOORD;
};

//Output from the domain shader
struct DomainOut
{
    float4 position     : SV_POSITION;
	float3 normal	    : NORMAL;
    float3 tangent      : TANGENT;
	float2 texCoord     : TEXCOORD;
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
	
	vout.position = mul(float4(p_input.position,1.0f), p_input.instanceTransform).xyz;
	vout.normal = mul(float4(p_input.normal,0.0f), p_input.instanceTransform).xyz;
	vout.tangent = mul(float4(p_input.tangent,0.0f),p_input.instanceTransform).xyz;
	vout.texCoord = p_input.texCoord;
    
	return vout;
}

//PATCH CONSTANT FUNCTION:
//Determines tesselation factors.
patchConstantOut PatchConstant(InputPatch<VertexOut, 3> input, uint patchID: SV_PrimitiveID)
{
	patchConstantOut output;
	
	//The amount of points added on each respective edge of the triangle
	output.edges[0] = 1;
	output.edges[1] = 1;
	output.edges[2] = 1;
	
	//The amount of points added INSIDE the triangle
	output.inside = 1;
	
	return output;
}

[domain("tri")] //We are tesselating triangles
[partitioning("integer")] //Partitioning using integer factors
[outputtopology("triangle_cw")] //Winding of triangles
[outputcontrolpoints(3)] //We want three control points (triangles).
[patchconstantfunc("PatchConstant")] //Identifier to the PatchConstant Function
HullOut HS(InputPatch<VertexOut, 3> patch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	HullOut output;
	output.position = patch[pointID].position;
	output.normal 	= patch[pointID].normal;
	output.tangent 	= patch[pointID].tangent;
	output.texCoord 	= patch[pointID].texCoord;
	return output;
}

[domain("tri")] //We are working with triangles
DomainOut DS(patchConstantOut input, float3 uvw : SV_DomainLocation, const OutputPatch<HullOut, 3> patch)
{
	//Compute output data based on interpolation
	//over the triangle domain
	float3 pos;
	float3 norm;
	float3 tan;
	float3 bin;
	float2 tex;
	pos = uvw.x * patch[0].position + uvw.y * patch[1].position + uvw.z * patch[2].position;
	norm = uvw.x * patch[0].normal + uvw.y * patch[1].normal + uvw.z * patch[2].normal;
	tan = uvw.x * patch[0].tangent + uvw.y * patch[1].tangent + uvw.z * patch[2].tangent;
	tex = uvw.x * patch[0].texCoord + uvw.y * patch[1].texCoord + uvw.z * patch[2].texCoord;
	
	//Normalize tangent base
	tan    = normalize(tan);
	bin    = normalize(bin);
	
	
	//Displacement mapping - Not used right now
	//float disp;
	//if (UseAlpha)
		//disp = DisplacementTexture.SampleLevel(SampleType, tex, 0).a;
	//else
		//disp = DisplacementTexture.SampleLevel(SampleType, tex, 0).r;
	
	//disp = (disp - 0.5f) * 2 * Displacement;
	//pos = pos + disp * norm;
	
	//Map output data
	DomainOut output;
	output.position = float4(pos,1.0f);
    output.position = mul(output.position, gViewProj);
	output.tangent = tan;
	output.normal = norm;
	output.texCoord = tex;
	return output;
}

PixelOut PS(DomainOut p_input)
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

