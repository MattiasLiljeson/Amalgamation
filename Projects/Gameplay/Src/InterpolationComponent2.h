#pragma once

#include "Component.h"
#include "Transform.h"
#include <deque>

struct InterData
{
	AglMatrix transform;
	float t;
	float stamp;
};

class InterpolationComponent2: public Component
{
public:
	InterpolationComponent2()
		: Component( ComponentType::InterpolationComponent2 )
	{
		t = 0;
		diff = AglMatrix::zeroMatrix();
		diffT = 1;
	}

public:
	deque<InterData> data;
	AglMatrix diff;
	float diffT;
	float t;

};