#include "Component.h"

int Component::m_counter = 0;

Component::Component()
{
	m_counter++;
	m_id = m_counter;
	m_componentTypeId = ComponentType::NON_EXISTING; 
}


Component::~Component()
{
}

void Component::init( vector<ComponentData> p_initData )
{
}

void Component::setComponentType( ComponentType::ComponentTypeIdx p_type )
{ 
	m_componentTypeId = p_type;
}

ComponentType::ComponentTypeIdx Component::getComponentTypeId()
{ 
	return m_componentTypeId;
}