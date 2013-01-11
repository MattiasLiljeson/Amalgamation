#pragma once

#include "ComponentType.h"
#include "ComponentData.h"

#include <vector>

using namespace std;


class Component
{
public:
	Component();
	virtual ~Component() = 0;

	virtual void init( vector<ComponentData> p_initData );
	virtual void setType( ComponentType::ComponentTypeIdx p_type );
	virtual ComponentType::ComponentTypeIdx getType();
protected:
	ComponentType::ComponentTypeIdx m_type;
	static int m_counter;
	int m_id;
};

