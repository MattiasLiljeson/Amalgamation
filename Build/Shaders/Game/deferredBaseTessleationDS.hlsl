#include "deferredBasePixelStruct.hlsl"
#include "patchConstantStruct.hlsl"
#include "tesselationStruct.hlsl"
#include "perFrameCBuffer.hlsl"

Texture2D displaceTexture : register(t4);

SamplerState pointSampler : register(s0);

[domain("tri")] //We are working with triangles
PixelIn DS(patchConstant input, float3 uvw : SV_DomainLocation, 
			const OutputPatch<TesselationStruct, 3> patch)
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
	
	
	float disp;
	disp = displaceTexture.SampleLevel(pointSampler, tex, 0).r;
	disp = (disp - 0.5f) * 2 * 0.5f;
	pos = pos + disp * norm;
	
	//Map output data
	PixelIn output;
	output.position = float4(pos,1.0f);
    output.position = mul(output.position, gViewProj);
	output.tangent = tan;
	output.normal = norm;
	output.texCoord = tex;
	output.gradientColor = patch[0].gradientColor;
	output.flags = patch[0].flags;
	return output;
}