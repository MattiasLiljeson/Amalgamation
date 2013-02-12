#include "ShipModule.h"

ComponentRegister<ShipModule> ShipModule::s_reg("ShipModule");

ShipModule::ShipModule()
	: Component( ComponentType::ShipModule )
{
	m_parentEntity = -1;
	m_active = false;
	m_value = 100;
	m_health = 100.0f;
	m_addedDamage = 0;
	m_rotationDirection = 0;
	m_rotation = 0;
}

ShipModule::~ShipModule()
{
	for(unsigned int i=0; i<m_activationEvents.size(); i++)
	{
		delete m_activationEvents[i];
	}
}

void ShipModule::init( vector<ComponentData> p_initData )
{
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		if( p_initData[i].dataName == "m_value" )
			p_initData[i].getData<float>(&m_value);
		else if( p_initData[i].dataName == "m_health" )
			p_initData[i].getData<float>(&m_health);
	}
	m_rotation = 0;
	m_rotationDirection = 0;
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

const bool& ShipModule::getActive() const
{
	return m_active;
}

void ShipModule::activate()
{
	m_active = true;
	for(unsigned int i=0; i<m_activationEvents.size(); i++)
	{
		m_activationEvents[i]->activate();
	}
}

void ShipModule::deactivate()
{
	m_active = false;
	for(unsigned int i=0; i<m_activationEvents.size(); i++)
	{
		m_activationEvents[i]->deactivate();
	}
}

void ShipModule::addActivationEvent( ModuleEvent* p_event )
{
	m_activationEvents.push_back( p_event );
}
