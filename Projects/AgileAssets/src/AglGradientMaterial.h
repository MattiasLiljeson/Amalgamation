#ifndef AGLGRADIENTMATERIAL_H
#define AGLGRADIENTMATERIAL_H

#include "AglVector3.h"

struct AglGradientMaterial
{
	AglVector3 ambient;
	AglVector3 diffuse;
	AglVector3 specular;
	AglVector3 emissive;
	float opacity;
	float reflectivity;
	float shininess;

	AglGradientMaterial();
	virtual ~AglGradientMaterial();
};

#endif