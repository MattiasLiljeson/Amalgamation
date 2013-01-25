#include "ShipModule.h"

ComponentRegister<ShipModule> ShipModule::s_reg("ShipModule");

ShipModule::ShipModule()
{
	m_parentEntity = -1;
	m_active = false;
	m_value = 100;
	m_health = 100.0f;
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