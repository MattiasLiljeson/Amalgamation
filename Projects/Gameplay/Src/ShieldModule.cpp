#include "ShieldModule.h"

ComponentRegister<ShieldModule> ShieldModule::s_reg("ShieldModule");

ShieldModule::ShieldModule()
	: Component( ComponentType::ShieldModule )
{
	maxRange = 100.0f;
	cooldown = 0.0f;
	cooldownTime = 1.0f;
	activationTime = 0.5f;
	activation = 0.0f;
	impulse = 100.0f;
}

void ShieldModule::init( vector<ComponentData> p_initData )
{
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "cooldownTime") {
			p_initData[i] >> cooldownTime;
		}
		else if(p_initData[i].dataName == "activationTime") {
			p_initData[i] >> activationTime;
		}
		else if(p_initData[i].dataName == "impulse") {
			p_initData[i] >> impulse;
		}
		else if(p_initData[i].dataName == "maxRange") {
			p_initData[i] >> maxRange;
		}
	}
}