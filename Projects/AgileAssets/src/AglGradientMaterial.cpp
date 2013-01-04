#include "AglGradientMaterial.h"

AglGradientMaterial::AglGradientMaterial()
{
	diffuse = AglVector3(0.5f, 0.5f, 0.5f);
	specular = AglVector3(1, 1, 1);
	ambient = emissive = AglVector3(0, 0, 0);
	opacity = 1.0f;
	reflectivity = 0.0f;
	shininess = 5.0f;
}
AglGradientMaterial::~AglGradientMaterial()
{
}