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

float3 pointLight(SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pixelPos, float3 lightPos)
{	
	// The vector from the surface to the light.
	float3 lightVec = lightPos - pixelPos;
	
	// The distance from surface to light.
	float d = length(lightVec);
	
	if( d > light.range )
	{
		return light.ambient.xyz;
		//float3 green = float3(0.0f, 1.0f, 0.0f);
		//return green;
	}
	
	// Normalize the light vector.
	lightVec /= d;
	
	// Add the ambient light term.
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	litColor += surface.diffuse.xyz * light.ambient.xyz;
	
	// Add diffuse and specular term, provided the surface is in
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		//return float3(0, 0, 0.5);
		float specPower = max(surface.specular.a, 1.0f);
		float3 toEye = normalize(eyePos - pixelPos);
		float3 R = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
		
		// diffuse and specular terms
		litColor += diffuseFactor.r * (surface.diffuse.rgb * light.diffuse.rgb);
		litColor += specFactor.r * (surface.specular.rgba * light.specular.rgba);
	}
	// attenuate
	return litColor / dot(light.attenuation, float3(1.0f, d, d*d));
}