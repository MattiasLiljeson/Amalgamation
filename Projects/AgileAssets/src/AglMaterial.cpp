#include "AglMaterial.h"

AglMaterial::AglMaterial()
{
	id = -1;
	diffuse = AglVector3(0.5f, 0.5f, 0.5f);
	specular = AglVector3(1, 1, 1);
	ambient = emissive = AglVector3(0, 0, 0);
	opacity = 1.0f;
	reflectivity = 0.0f;
	shininess = 5.0f;
	diffuseTextureNameIndex = -1;
	specularTextureNameIndex = -1;
	normalTextureNameIndex = -1;
	glowTextureNameIndex = -1;
	displacementTextureNameIndex = -1;
	displacement = 0;
	tesselationFactor = AglVector4(1, 1, 1, 1);
	textureScale = 1;
	gradientDataIndex = -1;
	gradientTextureNameIndex = -1;
}