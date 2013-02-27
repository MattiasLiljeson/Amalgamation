struct VertexIn
{
	float3 position : POSITION;	
	float3 normal 	: NORMAL;
	float2 texCoord : TEXCOORD; 
	float3 tangent 	: TANGENT;	
	float3 binormal : BINORMAL;
	
	//Per instance
	float4x4 instanceTransform  : INSTANCETRANSFORM;
	float4x4 gradientColor 		: GRADIENTCOLOR;
	float4 flags 	: FLAGS;
	float4 colorOverlay : OVERLAY;
};