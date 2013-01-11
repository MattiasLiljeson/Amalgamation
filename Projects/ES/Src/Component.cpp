#include "Component.h"

int Component::m_counter = 0;

Component::Component()
{
	m_counter++;
	m_id = m_counter;
	m_type = ComponentType::NON_EXISTING; 
}


Component::~Component()
{
}

void Component::init( vector<ComponentData> p_initData )
{
}

void Component::setType( ComponentType::ComponentTypeIdx p_type )
{ 
	m_type = p_type;
}

ComponentType::ComponentTypeIdx Component::getType()
{ 
	return m_type;
}