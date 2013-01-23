#ifndef AGLGRADIENTMATERIAL_H
#define AGLGRADIENTMATERIAL_H

#include <AglVector4.h>

struct AglGradientMaterial
{
	AglVector4 color;

	AglGradientMaterial();
	virtual ~AglGradientMaterial();
};

#endif