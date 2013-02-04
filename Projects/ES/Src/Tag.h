#pragma once

// Temporary tag class
#include "Component.h"


///
/// TAG
///
class Tag : public Component
{
public:
	Tag() : Component( ComponentType::Tag )
	{
	}

	virtual ~Tag() {}
};