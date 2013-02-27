struct PixelIn
{
    float4 position	: SV_POSITION;
	float2 texCoord	: TEXCOORD;
	float3 normal 	: NORMAL;
	float3 tangent 	: TANGENT;
	float4x4 gradientColor : GRADIENTCOLOR;
	float4 flags	: FLAGS;
	float4 colorOverlay	: OVERLAY;
};