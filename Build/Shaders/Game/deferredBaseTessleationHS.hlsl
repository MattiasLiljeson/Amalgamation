#include "tesselationStruct.hlsl"
#include "perFrameCBuffer.hlsl"
#include "patchConstantStruct.hlsl"

//PATCH CONSTANT FUNCTION:
//Determines tesselation factors.
patchConstant PatchConstant(InputPatch<TesselationStruct, 3> input, 
	uint patchID: SV_PrimitiveID)
{
	patchConstant output;
	
	float3 pos = (input[0].position + input[1].position + input[2].position) / 3.0f;
	
	float3 toSurface = pos - gCameraPos.xyz;
	
	float len = length(toSurface);
	
	int factor = 1;
	
	float maxTess = 10.0f;
	if (len < 200)
	{
		factor += 0.005*len*maxTess;
	}
	
	//The amount of points added on each respective edge of the triangle
	output.edges[0] = factor;
	output.edges[1] = factor;
	output.edges[2] = factor;
	
	//The amount of points added INSIDE the triangle
	output.inside = factor;
	
	return output;
}

[domain("tri")] //We are tesselating triangles
[partitioning("integer")] //Partitioning using integer factors
[outputtopology("triangle_cw")] //Winding of triangles
[outputcontrolpoints(3)] //We want three control points (triangles).
[patchconstantfunc("PatchConstant")] //Identifier to the PatchConstant Function
TesselationStruct HS(InputPatch<TesselationStruct, 3> patch, uint pointID : SV_OutputControlPointID, 
			uint patchID : SV_PrimitiveID)
{
	return patch[pointID];
}