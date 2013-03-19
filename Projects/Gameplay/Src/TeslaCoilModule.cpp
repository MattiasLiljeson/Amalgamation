#include "TeslaCoilModule.h"

ComponentRegister<TeslaCoilModule> TeslaCoilModule::s_reg("TeslaCoilModule");

TeslaCoilModule::TeslaCoilModule()
	: Component(ComponentType::TeslaCoilModule)
{
	range = 10.0f;
	optimalRange = 5.0f;
	damage = 0.0f;
	cooldownTime = 10.0f;
	cooldown = 0.0f;
	active = false;
}

TeslaCoilModule::TeslaCoilModule( float p_range, float p_optimalRange,
	float p_cooldownTime, float p_damage )
	: Component(ComponentType::TeslaCoilModule)
{
	range = p_range;
	optimalRange = p_optimalRange;
	cooldownTime = p_cooldownTime;
	damage = p_damage;
	cooldown = 0.0f;
	active = false;
}

void TeslaCoilModule::init( vector<ComponentData> p_initData )
{
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "range") {
			p_initData[i] >> range;
		}
		else if(p_initData[i].dataName == "optimalRange") {
			p_initData[i] >> optimalRange;
		}
		else if(p_initData[i].dataName == "cooldownTime") {
			p_initData[i] >> cooldownTime;
		}
		else if(p_initData[i].dataName == "damage") {
			p_initData[i] >> damage;
		}
	}
}