#include "LightBlinker.h"

ComponentRegister<LightBlinker> LightBlinker::s_reg("LightBlinker");


LightBlinker::LightBlinker()
{
	increase = true;
	maxRange = 1.0f;
	m_componentType = ComponentType::ComponentTypeIdx::LightBlinker;
}

LightBlinker::LightBlinker(float p_maxRange)
{
	increase = true;
	maxRange = p_maxRange;
	m_componentType = ComponentType::ComponentTypeIdx::LightBlinker;
}
void LightBlinker::init( vector<ComponentData> p_initData )
{
	char* cStr;
	int temp=0;
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		if( p_initData[i].dataName == "maxrange" )
			p_initData[i].getData<float>(&maxRange);
		else if( p_initData[i].dataName == "increase" )
		{
			p_initData[i].getData<int>(&temp);
			increase = temp==1?true:false;
		}
	}
}