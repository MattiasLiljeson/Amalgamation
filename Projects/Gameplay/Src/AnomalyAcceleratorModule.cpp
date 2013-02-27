#include "AnomalyAcceleratorModule.h"

ComponentRegister<AnomalyAcceleratorModule> AnomalyAcceleratorModule::
	s_reg("AnomalyAcceleratorModule");

AnomalyAcceleratorModule::AnomalyAcceleratorModule()
	: Component(ComponentType::AnomalyAcceleratorModule)
{
	cooldown = 0.0f;
	cooldownTime = 1.0f;
}

void AnomalyAcceleratorModule::init( vector<ComponentData> p_initData )
{
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "cooldownTime") {
			p_initData[i] >> cooldownTime;
		}
	}
}