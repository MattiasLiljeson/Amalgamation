#pragma once

#include <Component.h>
#include <DebugUtil.h>

class PickComponent : public Component
{
public:
	PickComponent()
	{
		m_rayIndex = -1;
	}

	~PickComponent() {}

	int m_rayIndex;
};