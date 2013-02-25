#pragma once

#include <Component.h>
#include <DebugUtil.h>

class SelectionMarker : public Component
{
public:
	SelectionMarker()
		: Component( ComponentType::SelectionMarker )
	{
		m_targetEntity = -1;
	}

	~SelectionMarker() {}

	int m_targetEntity;
};