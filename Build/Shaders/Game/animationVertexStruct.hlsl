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
	
	//Per instance
	float4x4 instanceTransform 	: INSTANCETRANSFORM;
	float4x4 gradientColor : GRADIENTCOLOR;
	float4 flags : FLAGS; //1: Number of gradient levels 		2: Number of bones   	3-4: Empty
	uint instanceId : SV_InstanceID;
};