struct SurfaceLightingData
{
    float4 diffuseColor;
    float4 specularColor;
};

struct Light
{
	float diffusePower;
	float specularPower;
	float4 vec; // direction(w=0) or position(w=1)
	float4 diffuseColor;
	float4 specularColor;
};

SurfaceLightingData Lambert(Light p_light, float4 p_surfaceNormal)
{
	SurfaceLightingData result;
	float val=dot(p_light.vec, p_surfaceNormal)*p_light.diffusePower;
	result.diffuseColor = float4(val,val,val,0.0f);
	result.specularColor = float4(0.0f,0.0f,0.0f,0.0f);
	return result;
}

// New light structs and algorithms /Mattias L
struct LightInfo
{
	float3 	pos;
	float3 	lightDir				: LIGHTDIR;
	float 	range					: RANGE;
	float3 	attenuation				: ATTENUATION;
	float 	spotLightConeSizeAsPow	: SPOTLIGHTCONESIZEASPOW;
	float3 	color					: COLOR;
	float 	lightEnergy				: LIGHTCOLOR;
	int 	enabled 				: ENABLED;
	int 	type 					: TYPE;
	int		shadowIdx				: SHADOWIDX;
	//float2 	padding;
};

struct SurfaceInfo
{
	float4	diffuse;
	float4	specular;
};

struct LightOut
{
	float3 lightDiffuse;
	float3 lightSpecular;
};

struct PixelOut
{
	float4 lightDiffuse 	: SV_TARGET0;
	float4 lightSpecular 	: SV_TARGET1;
};

LightOut parallelLight( LightInfo light, float3 eyePos, float3 normal, float3 pixelPos, float gloss )
{
	
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -light.lightDir;
	
	LightOut lightOut;
	lightOut.lightDiffuse = 0.0f;
	lightOut.lightSpecular = 0.0f;
	
	// Add diffuse and specular term, provided the surface is in
	// the line of sight of the light.
	float diffuseFactor = dot( lightVec, normal );
	[branch]
	if( diffuseFactor > 0.0f ) {
		float specPower = max( gloss*255.0f, 1.0f );
		float3 toEye = normalize( eyePos - pixelPos );
		float3 R = reflect(-lightVec, normal);
		float specFactor = pow( max( dot(R, toEye), 0.0f ), specPower );
		
		// diffuse and specular terms
		lightOut.lightDiffuse += diffuseFactor * light.color.rgb;
		lightOut.lightSpecular += specFactor * light.color.rgb;
	}
	return lightOut;
}

LightOut pointLight( LightInfo light, float3 eyePos, float3 normal, float3 pixelPos, float gloss  )
{	
	// The vector from the surface to the light.
	float3 lightVec = light.pos - pixelPos;

	// The distance from surface to light.
	float d = length( lightVec );

	LightOut lightOut;
	lightOut.lightDiffuse = 0.0f;
	lightOut.lightSpecular = 0.0f;
	if( d > light.range ) {
		return lightOut;
	}

	// Normalize the light vector.
	lightVec /= d;

	// Add diffuse and specular term, provided the surface is in
	// the line of sight of the light.
	float diffuseFactor = dot( lightVec, normal );
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower = max( gloss*255.0f, 1.0f );
		float3 toEye = normalize( eyePos - pixelPos );
		float3 R = reflect( -lightVec, normal );
		float specFactor = pow( max( dot(R, toEye), 0.0f ), specPower );
		
		// diffuse and specular terms
		lightOut.lightDiffuse += diffuseFactor * light.color.rgb;
		lightOut.lightSpecular += specFactor * light.color.rgb;
	}

	// Attenuate, Blender std-values
	float linAtt = 0.0f;
	float quadAtt = 1.0f;
	float lin = light.range / (light.range + linAtt.r * d);
	float rangeSqr = light.range * light.range;
	float quad = rangeSqr / (rangeSqr + quadAtt * d*d); 

	// Log falloff near edge of light
	float rangeClamped = (d / light.range);
	float falloff = 1 - (rangeClamped * rangeClamped * rangeClamped);
	falloff = saturate(falloff);

	// Blender cheat non-physical nonsenese value ;-)
	float energy = light.lightEnergy;
	float intensity = falloff * energy * lin * quad;

	lightOut.lightDiffuse *= intensity;
	lightOut.lightSpecular *= intensity;

	return lightOut;
}

LightOut spotLight( LightInfo light, float3 eyePos, float3 normal, float3 pixelPos, float gloss )
{
	LightOut lightOut = pointLight( light, eyePos, normal, pixelPos, gloss );

	// The vector from the surface to the light.
	float3 lightVec = normalize( light.pos - pixelPos );

	float spotFactor = pow( max( dot(-lightVec, light.lightDir), 0.0f ), light.spotLightConeSizeAsPow );

	// Scale color by spotlight factor.
	lightOut.lightDiffuse *= spotFactor;
	lightOut.lightSpecular *= spotFactor;
	return lightOut;
}