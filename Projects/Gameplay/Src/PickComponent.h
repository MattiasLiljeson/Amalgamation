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
		m_selection = -1;
		m_targetEntity = -1;
		m_targetSlot = -1;
	}

	~PickComponent() {}

	int m_rayIndex;
	int m_clientIndex;
	int m_latestPick;
	bool m_active;
	float m_preferredDistance;
	int m_selection;

	int m_targetEntity;
	int m_targetSlot;
};