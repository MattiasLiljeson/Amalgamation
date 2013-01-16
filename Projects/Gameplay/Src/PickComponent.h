#pragma once

#include <Component.h>
#include <DebugUtil.h>

class PickComponent : public Component
{
public:
	PickComponent()
	{
		m_rayIndex = -1;
		m_latestPick = -1;
		m_clientIndex = -1;
		m_active = false;
		m_preferredDistance = 0;
	}

	~PickComponent() {}

	int m_rayIndex;
	int m_clientIndex;
	int m_latestPick;
	bool m_active;
	float m_preferredDistance;
};