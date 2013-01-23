#include "ShipModule.h"

ShipModule::ShipModule()
{
	m_parentEntity = -1;
	m_active = false;
	m_value = 100;
	m_health = 100.0f;
}

ShipModule::~ShipModule()
{

}