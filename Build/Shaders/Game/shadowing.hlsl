float doShadowing( Texture2D tex, SamplerState p_pointSampler, float4 positionHomo ){
	float SHADOW_EPSILON = 1.0f;
	float SMAP_SIZE = 512.0f;
	
	positionHomo.xyz /= positionHomo.w;
	if(positionHomo.z >= 1.0f)
		return 1.0f;
	
	float2 smTexCoord = float2(0.5f*positionHomo.x, -0.5f*positionHomo.y)+0.5f;

	float dx = 1.0f/SMAP_SIZE;
	float depth = positionHomo.z * SHADOW_EPSILON;
	
	if (smTexCoord.x < 0.01f || smTexCoord.y <0.01f || smTexCoord.x+dx >= 0.99f || smTexCoord.y+dx >= 0.99f)
		return 1.0f;
	
	float s0 = (tex.Sample(p_pointSampler, smTexCoord).r < depth)? 0.0f: 1.0f;
	float s1 = (tex.Sample(p_pointSampler, smTexCoord + float2 (dx,0.0f)).r < depth)? 0.0f: 1.0f;
	float s2 = (tex.Sample(p_pointSampler, smTexCoord + float2 (0.0f,dx)).r < depth)? 0.0f: 1.0f;
	float s3 = (tex.Sample(p_pointSampler, smTexCoord + float2 (dx,dx)).r < depth)? 0.0f: 1.0f;
	
	float2 texelPos = smTexCoord * SMAP_SIZE;
	float2 lerps = frac(texelPos);
	
	return float (lerp(lerp(s0,s1,lerps.x),lerp(s2,s3,lerps.x),lerps.y));
}