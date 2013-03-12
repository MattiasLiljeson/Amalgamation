#include "Component.h"

int Component::m_counter = 0;

Component::Component( ComponentType::ComponentTypeIdx p_componentType )
{
	m_counter++;
	m_id = m_counter;
	m_componentTypeId = p_componentType;
}

Component::~Component()
{
}

void Component::init( vector<ComponentData> p_initData )
{
}

void Component::setComponentTypeId( ComponentType::ComponentTypeIdx p_type )
{ 
	m_componentTypeId = p_type;
}

ComponentType::ComponentTypeIdx Component::getComponentTypeId()
{ 
	return m_componentTypeId;
}