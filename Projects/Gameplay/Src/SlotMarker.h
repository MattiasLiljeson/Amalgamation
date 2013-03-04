#pragma once

#include <Component.h>
#include "EntityType.h"

class SlotMarker : public Component
{
public:
	SlotMarker()
		: Component( ComponentType::SlotMarker )
	{ 
		shipMarker = -1;
		dir = AglVector3(1, 0, 0);
	};
	~SlotMarker(){}

	int shipMarker;
	AglVector3 dir;
	vector<pair<EntityType, Entity*>> m_collection;
};