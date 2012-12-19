#include "Component.h"

int Component::m_counter = 0;

Component::Component()
{
	m_counter++;
	m_id = m_counter;
}


Component::~Component()
{
}
