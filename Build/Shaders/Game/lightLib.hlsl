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

// TODO: Implement Phong, Blinn-phong and maybe more?


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
	//float3	normal;
	//float	pad1;
	//float3	pos;
	//float	pad2;
};

float3 parallelLight( SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pixelPos )
{
	
	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -light.lightDir;
	
	// Add the ambient term.
	float3 litColor = float3( 0.0f, 0.0f, 0.0f );
	litColor += surface.diffuse.xyz * light.color.xyz;
	
	// Add diffuse and specular term, provided the surface is in
	// the line of site of the light.
	float diffuseFactor = dot( lightVec, normal );
	[branch]
	if( diffuseFactor > 0.0f ) {
		float specPower = max( surface.specular.a, 1.0f );
		float3 toEye = normalize( eyePos - pixelPos );
		float3 R = reflect(-lightVec, normal);
		float specFactor = pow( max( dot(R, toEye), 0.0f ), specPower );
		
		// diffuse and specular terms
		litColor += diffuseFactor * surface.diffuse.xyz * light.color.xyz;
		litColor += specFactor * surface.specular.xyz * light.color.xyz;
	}
	return float3(0,0,0);
	return litColor;
}

float3 pointLight( SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pixelPos )
{	
	// The vector from the surface to the light.
	float3 lightVec = light.pos - pixelPos;

	// The distance from surface to light.
	float d = length( lightVec );

	if( d > light.range ) {
		return float3( 0.0f, 0.0f, 0.0f );
	}

	// Normalize the light vector.
	lightVec /= d;

	// Add diffuse and specular term, provided the surface is in
	// the line of sight of the light.
	float3 litColor = float3( 0.0f, 0.0f, 0.0f );
	float diffuseFactor = dot( lightVec, normal );
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower = max( surface.specular.a, 1.0f );
		float3 toEye = normalize( eyePos - pixelPos );
		float3 R = reflect( -lightVec, normal );
		float specFactor = pow( max( dot(R, toEye), 0.0f ), specPower );
		
		// diffuse and specular terms
		litColor += diffuseFactor * (surface.diffuse.rgb * light.color.rgb);
		litColor += specFactor * (surface.specular.rgb * light.color.rgb);
	}

	// Attenuate
	// Blender std-values
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

	return litColor * intensity;
}

float3 spotLight( SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pixelPos )
{
	//return float3( 0, 0.5, 0 );
	//return surface.diffuse.xyz * 0.5f;
	float3 litColor = pointLight(surface, light, eyePos, normal, pixelPos);

	//litColor = surface.diffuse.xyz;
	// The vector from the surface to the light.
	float3 lightVec = normalize(light.pos - pixelPos);
	//lightVec = float3( 0, 0, -1 );
	//litColor = float3( 0, 0.5, 0 );
	//light.spotLightConeSizeAsPow = 8.0;

	float s = pow( max( dot(-lightVec, light.lightDir), 0.0f ), light.spotLightConeSizeAsPow );

	// Scale color by spotlight factor.
	//return float3(0,0,0);
	return litColor*s;
}