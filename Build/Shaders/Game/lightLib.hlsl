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
	//float4x4 lightViewProj; // For shadowmapgeneration;
	float3 	pos;
	float 	range		: RANGE;
	float3 	lightDir	: LIGHTDIR;
	float3 	attenuation	: ATTENUATION;
	float 	spotPower	: SPOTPOWER;
	float4 	ambient		: AMBIENT;
	float4 	diffuse		: DIFFUSE;
	float4 	specular	: SPECULAR;
	int 	enabled 	: ENABLED;
	int 	type 		: TYPE;
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
	litColor += surface.diffuse.xyz * light.ambient.xyz;
	
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
		litColor += diffuseFactor * surface.diffuse.xyz * light.diffuse.xyz;
		litColor += specFactor * surface.specular.xyz * light.specular.xyz;
	}

	return litColor;
}

float3 pointLight( SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pixelPos )
{	
	//return surface.diffuse.xyz;

	// lulz tonemapping
	//surface.diffuse *=  float4( 1.1, 0.8, 0.5, 1.0f );
	
	// The vector from the surface to the light.
	float3 lightVec = light.pos - pixelPos;
	
	// The distance from surface to light.
	float d = length( lightVec );
	
	if( d > light.range )
	{
		return light.ambient.xyz;
		//float3 green = float3(0.0f, 1.0f, 0.0f);
		//return green;
	}
	
	// Normalize the light vector.
	lightVec /= d;
	
	// Add the ambient light term.
	float3 litColor = float3( 0.0f, 0.0f, 0.0f );
	litColor += surface.diffuse.xyz * light.ambient.xyz;
	
	// Add diffuse and specular term, provided the surface is in
	// the line of site of the light.
	
	float diffuseFactor = dot( lightVec, normal );
	[branch]
	if( diffuseFactor > 0.0f )
	{
		//return float3(0, 0, 0.5);
		float specPower = max( surface.specular.a, 1.0f );
		float3 toEye = normalize( eyePos - pixelPos );
		float3 R = reflect( -lightVec, normal );
		float specFactor = pow( max( dot(R, toEye), 0.0f ), specPower );
		
		// diffuse and specular terms
		litColor += diffuseFactor * (surface.diffuse.rgb * light.diffuse.rgb);
		litColor += specFactor * (surface.specular.rgb * light.specular.rgb);
	}
	// attenuate
	return litColor / dot( light.attenuation, float3(1.0f, d, d*d) );
}

float3 spotLight( SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pixelPos )
{
	//return surface.diffuse.xyz * 0.5f;
	float3 litColor = pointLight(surface, light, eyePos, normal, pixelPos);

	//litColor = surface.diffuse.xyz;
	
	// The vector from the surface to the light.
	float3 lightVec = normalize(light.pos - pixelPos);

	float s = pow( max( dot(-lightVec, light.lightDir), 0.0f ), light.spotPower );

	// Scale color by spotlight factor.
	return litColor*s;
}