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
	}

	~PickComponent() {}

	int m_rayIndex;
	int m_clientIndex; ///< owner client index
	int m_latestPick; ///< the current picked entity
	bool m_active;
	float m_preferredDistance;
	int m_selection; ///< index to selection sphere

	int m_targetEntity; ///< current entity to receive latest pick if released
	int m_targetSlot; ///< idx to current target connection point of targetentity
};