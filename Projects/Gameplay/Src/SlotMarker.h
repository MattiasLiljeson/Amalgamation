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
	};
	~SlotMarker(){}

	int shipMarker;
	vector<pair<EntityType, int>> m_collection;
};