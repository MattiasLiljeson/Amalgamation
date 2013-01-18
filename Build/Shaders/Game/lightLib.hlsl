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
	int 	type;
	float3 	dir;
	float 	range;
	float3 	att;
	float 	spotPower;
	float4 	ambient;
	float4 	diffuse;
	float4 	spec;
	int 	enabled;
	float3 	padding;
};

struct SurfaceInfo
{
	float4	diffuse;
	float4	spec;
	//float3	normal;
	//float	pad1;
	//float3	pos;
	//float	pad2;
};

float3 pointLight(SurfaceInfo surface, LightInfo light, float3 eyePos, float3 normal, float3 pos)
{	
	// The vector from the surface to the light.
	float3 lightVec = light.pos - pos;
	
	// The distance from surface to light.
	float d = length(lightVec);
	
	if( d > light.range )
	return float3(0.0f, 0.0f, 0.0f);
	
	// Normalize the light vector.
	lightVec /= d;
	
	// Add the ambient light term.
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	litColor += surface.diffuse * light.ambient;
	
	// Add diffuse and specular term, provided the surface is in
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, normal);
	[branch]
	if( diffuseFactor > 0.0f )
	{
		float specPower = max(surface.spec.a, 1.0f);
		float3 toEye = normalize(eyePos - pos);
		float3 R = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);
		
		// diffuse and specular terms
		litColor += diffuseFactor * surface.diffuse * light.diffuse;
		litColor += specFactor * surface.spec * light.spec;
	}
	
	// attenuate
	return litColor / dot(light.att, float3(1.0f, d, d*d));
}