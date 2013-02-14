#pragma once

#include <Component.h>
#include <DebugUtil.h>

class PickComponent : public Component
{
public:
	PickComponent()
	: Component( ComponentType::PickComponent )
	{
		m_rayIndex = -1;
		m_latestPick = -1;
		m_clientIndex = -1;
		m_active = false;
		m_preferredDistance = 0;
		m_selection = -1;
		m_targetEntity = -1;
		m_targetSlot = -1;
		m_latestAttached = -1;
		m_rotationDirection = 0;
	}

	~PickComponent() {}

	void setLatestPick(int p_latestPick)
	{
		m_latestPick = p_latestPick;
		if (m_latestPick >= 0)
			m_latestAttached = -1;
	}
	int getLatestPick()
	{
		return m_latestPick;
	}

	int m_rayIndex;
	int m_clientIndex;
	bool m_active;
	float m_preferredDistance;
	int m_selection;

	int m_targetEntity;
	int m_targetSlot;

	//Used for rotation
	int m_latestAttached;
	int m_rotationDirection;

private:
	int m_latestPick;
};