#include "ShipModule.h"

ComponentRegister<ShipModule> ShipModule::s_reg("ShipModule");

ShipModule::ShipModule()
	: Component( ComponentType::ShipModule )
{
	m_parentEntity = -1;
	m_lastShipEntityWhenAttached = -1;
	m_active = false;
	m_value = 100;
	m_health = 100.0f;
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

void ShipModule::addDamageThisTick( float p_amount,int p_perpClientId )
{
	m_damageAcc.accumulatedDamage += p_amount;
	m_damageAcc.latestPerp = p_perpClientId;
}

void ShipModule::applyDamage()
{
	m_health -= m_damageAcc.accumulatedDamage;
	m_damageAcc.reset();
}

void ShipModule::resetDamage()
{
	m_damageAcc.reset();
}

bool ShipModule::damageTaken() const
{
	return m_damageAcc.accumulatedDamage > 0;
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

int ShipModule::getLatestPerpetratorClient()
{
	return m_damageAcc.latestPerp;
}
