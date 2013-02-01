#include "ShipModule.h"

ComponentRegister<ShipModule> ShipModule::s_reg("ShipModule");

ShipModule::ShipModule()
{
	m_parentEntity = -1;
	m_active = false;
	m_value = 100;
	m_health = 100.0f;
	m_addedDamage = 0;
	m_componentType = ComponentType::ShipModule;
}

ShipModule::~ShipModule()
{

}

void ShipModule::init( vector<ComponentData> p_initData )
{
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		if( p_initData[i].dataName == "value" )
			p_initData[i].getData<float>(&m_value);
		else if( p_initData[i].dataName == "health" )
			p_initData[i].getData<float>(&m_health);
	}
}

void ShipModule::addDamageThisTick( float p_amount )
{
	m_addedDamage += p_amount;
}

void ShipModule::applyDamage()
{
	m_health -= m_addedDamage;
	m_addedDamage = 0;
}

void ShipModule::resetDamage()
{
	m_addedDamage = 0;
}

bool ShipModule::damageTaken() const
{
	return m_addedDamage > 0;
}
