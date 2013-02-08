#pragma once

#include "ComponentType.h"
#include "ComponentData.h"

#include <vector>

using namespace std;


class Component
{
public:
	Component( ComponentType::ComponentTypeIdx p_componentType );
	virtual ~Component() = 0;

	virtual void init( vector<ComponentData> p_initData );
	virtual void setComponentType( ComponentType::ComponentTypeIdx p_type );
	virtual ComponentType::ComponentTypeIdx getComponentTypeId();
protected:
	ComponentType::ComponentTypeIdx m_componentTypeId;
	static int m_counter;
	int m_id;
};

