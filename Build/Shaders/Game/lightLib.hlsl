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
	float 	range		: RANGE;
	float3 	lightDir	: LIGHTDIR;
	float3 	attenuation	: ATTENUATION;
	float 	spotPower	: SPOTPOWER;
	float4 	ambient		: AMBIENT;
	float4 	diffuse		: DIFFUSE;
	float4 	specular	: SPECULAR;
	int 	enabled 	: ENABLED;
	int 	type 		: TYPE;
	int		shadowIdx	: SHADOWIDX;
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
	return float3(1,1,1);
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
	float dist = length( lightVec );
	
	if( dist > light.range )
	{
		//return float3(0,0.2,0);
		//return float3(0.0026,0.00035,0.0021);
		return float3(0,0,0);
		//return light.ambient.xyz;
		//float3 green = float3(0.0f, 1.0f, 0.0f);
		//return green;
	}
	// Normalize the light vector.
	lightVec /= dist;
	
	// Add the ambient light term.
	float3 litColor = float3( 0.0f, 0.0f, 0.0f );

	//litColor += surface.diffuse.xyz * light.ambient.xyz;
	//litColor += surface.diffuse.xyz * float3(0.013,0.00035,0.010);

	// Add diffuse and specular term, provided the surface is in
	// the line of site of the light.


	//float attenuation = 1.0f / light.range;
	//attenuation = attenuation * pow(clampedCosine, 
	
	//attenuation = 1.0f / (1.0f +  light.attenuation.y*dist+ light.attenuation.z*dist*dist)*2;
	float diffuseFactor = dot( lightVec, normal );
	[branch]
	if( diffuseFactor > 0.0f )
	{
		//return float3(0, 0, 0.5);
		float specPower = max( surface.specular.a, 1.0f );
		float3 toEye = normalize( eyePos - pixelPos );
		float3 R = reflect( -lightVec, normal );
		float specFactor = pow( max( dot(R, toEye), 0.0f ), specPower );
		float attenuation;
		attenuation = ((light.range*light.range / ( light.range*light.range + light.attenuation.z*R*R)));
		//float lightIntensity
		// diffuse and specular terms
		litColor += saturate(diffuseFactor * (surface.diffuse.rgb * light.diffuse.rgb* attenuation));
		litColor += saturate(specFactor * (surface.specular.rgb * light.specular.rgb* attenuation));
	}
	//litColor = attenuation * litColor;
	//litColor = float3(0,0,0);
	return litColor;
	// attenuate
	//litColor = litColor / dot( float3(0,0.00,0.00005), float3(1.0f, dist, dist*dist) );
	litColor = litColor / dot( float3(0,0.00,0.000015), float3(1.0f, dist, dist*dist) );
	
	//litColor = litColor / dot( light.attenuation, float3(1.0f, dist, dist*dist) );
	return litColor;
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