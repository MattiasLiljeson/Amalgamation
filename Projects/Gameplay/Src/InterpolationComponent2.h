#pragma once

#include "Component.h"
#include "Transform.h"

class InterpolationComponent2: public Component
{
public:
	InterpolationComponent2()
		: Component( ComponentType::InterpolationComponent2 )
	{
		target = AglMatrix::identityMatrix();
		source = AglMatrix::identityMatrix();
		start = 0;
		end = 0;
		t = 0;
	}

public:
	AglMatrix target;
	AglMatrix source;

	float start;
	float end;
	float t;

};