#include "Component.h"

int Component::m_counter = 0;

Component::Component( ComponentType::ComponentTypeIdx p_componentType )
{
	m_counter++;
	m_id = m_counter;
	m_componentType = p_componentType;
}

Component::~Component()
{
}

void Component::init( vector<ComponentData> p_initData )
{
}

void Component::setComponentType( ComponentType::ComponentTypeIdx p_type )
{ 
	m_componentType = p_type;
}

ComponentType::ComponentTypeIdx Component::getComponentType()
{ 
	return m_componentType;
}