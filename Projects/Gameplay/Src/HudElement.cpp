#include "HudElement.h"


HudElement::HudElement( string p_element )
{
	m_element = p_element;
	m_changed = true;
	m_value = "0";
}


HudElement::~HudElement()
{
}


void HudElement::setValue( string p_value )
{
	if( p_value != m_value )
	{
		m_value = p_value;
		m_changed = true;
	}
}

bool HudElement::hasChanged()
{
	return m_changed;
}

void HudElement::setRead()
{
	m_changed = false;
}

string HudElement::getElement()
{
	return m_element;
}

string HudElement::getValue()
{
	return m_value;
}