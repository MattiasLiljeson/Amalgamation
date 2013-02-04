

float doShadowing( Texture2D tex, SamplerState pointSampler, float4 positionHomo ){
	float SHADOW_EPSILON = 0.0001f;
	float SMAP_SIZE = 512.0f;
	
	positionHomo.xyz /= positionHomo.w;
	
	float2 smTexCoord = float2(0.5f*positionHomo.x, -0.5f*positionHomo.y)+0.5f;
	
	if (smTexCoord.x < 0.0f || smTexCoord.x > 1.0f || smTexCoord.y < 0.0f || smTexCoord.y > 1.0f )
		return 1.0f;
	
	float depth = positionHomo.z;
	
	float dx = 1.0f/SMAP_SIZE;
	float s0 = (tex.Sample(pointSampler, smTexCoord).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s1 = (tex.Sample(pointSampler, smTexCoord + float2 (dx,0.0f)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s2 = (tex.Sample(pointSampler, smTexCoord + float2 (0.0f,dx)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	float s3 = (tex.Sample(pointSampler, smTexCoord + float2 (dx,dx)).r + SHADOW_EPSILON<depth)? 0.0f: 1.0f;
	
	float2 texelPos = smTexCoord * SMAP_SIZE;
	float2 lerps = frac(texelPos);
	
	return float (lerp(lerp(s0,s1,lerps.x),lerp(s2,s3,lerps.x),lerps.y));
	//return s0;
}