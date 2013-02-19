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
	return float3(0,0,0);
	return litColor;
}

float3 pointLight( SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pixelPos )
{	
	float3 lightVec = light.pos - pixelPos;
	float dist = length( lightVec );
	lightVec /= dist;
	float range = light.range*0.5;
	float3 eyeVec = eyePos - pixelPos;
	float energy = light.spotPower;

	float3 litColor = float3( 0.0f, 0.0f, 0.0f );
	if(dist > range*2.5)
	{
		return litColor;
	}
	
	[branch]
	float diffuseIntensity = saturate(dot(normal,lightVec));
	float attenuation =  energy* 2* (range*range / ( range*range + dist*dist))*(range/(range+5.0*dist));
	//float intensity = attenuation * ( range - lightVec) / range;
	float3 h = normalize(lightVec + eyeVec);
	float averageStrengthSpec = ((surface.specular.r+surface.specular.g+surface.specular.b)/3);
	float specLighting = pow(saturate(dot(normal, h)), averageStrengthSpec*20);

	float3 diffuse = surface.diffuse.rgb*attenuation*saturate(surface.diffuse.rgb+light.diffuse.rgb)*diffuseIntensity*0.8;
	float3 specularity = surface.specular.rgb*specLighting*attenuation*light.diffuse.rgb*0.8;
	return max(0.0f,(diffuse+specularity));
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
	return float3(0,0,0);
	return litColor*s;
}