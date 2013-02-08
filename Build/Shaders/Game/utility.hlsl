float3 convertSampledNormal( float3 p_sampledNormal )
{
	// From 0.0 - 1.0 to -1.0 - 1.0
	return normalize(float3( p_sampledNormal * 2.0f - 1.0f ));
}

float2 getNdcPos( float2 p_clipSpacePos, float2 p_screenDim )
{
	float2 ndcPos;
	ndcPos = p_clipSpacePos.xy / p_screenDim.xy;
	ndcPos.y = 1.0f - ndcPos.y; 		// Flip y-axis
	ndcPos += float2( -0.5f, -0.5f ); 	// move origin to center
	ndcPos *= 2.0f; 					//expand to -1.0 - 1.0 from -0.5 - 0.5
	
	return ndcPos;
}

float3 getWorldPos( float2 p_ndcPos, float p_depth, float4x4 p_viewProjInv )
{
	float4 wvpPos = float4(	p_ndcPos.x, p_ndcPos.y, p_depth, 1.0f );
	
	float4 worldPos = mul( wvpPos, p_viewProjInv );
	return worldPos.xyz / worldPos.w;
}

float3 getWorldPosFromTexCoord( float2 texCoord, float depthValue, matrix invVP )
{
	float4 newTexCoord = float4 (  	texCoord.x*2.0f-1.0f, 
									(1.0f-texCoord.y)*2.0f-1.0f,
									depthValue, 
									1.0f);
	float4 worldPos;	
	worldPos	= mul(newTexCoord, invVP);
	
	return worldPos.xyz/worldPos.w;
}