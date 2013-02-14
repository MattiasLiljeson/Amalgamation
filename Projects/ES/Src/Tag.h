#pragma once

// Temporary tag class
#include "Component.h"


///
/// TAG
///
class Tag : public Component
{
public:
	Tag( ComponentType::ComponentTypeIdx p_type ) : Component( p_type )
	{
	}

	virtual ~Tag() {}
};