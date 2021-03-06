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
		m_targetEntity = -1;
		m_targetSlot = -1;
		m_latestAttached = -1;
		m_rotationDirection = 0;
		m_lastHovered = -1;
		m_desiredSlot = 0;
		m_inEditMode = false;
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
	int m_clientIndex; ///< owner client index
	bool m_active;
	float m_preferredDistance;
	int m_selection; ///< index to selection sphere

	int m_targetEntity; ///< current entity to receive latest pick if released
	int m_targetSlot; ///< idx to current target connection point of targetentity

	int m_desiredSlot; ///< desiredSlot on the module to be placed.

	//Used for rotation
	int m_latestAttached;
	int m_rotationDirection;

	int m_lastHovered; ///< Latest entity the pick component hovered over

	bool m_inEditMode; ///< Tells the pick whether or not it is in edit mode

private:
	int m_latestPick; ///< the current picked entity
};