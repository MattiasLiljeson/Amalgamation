#include "AnomalyAcceleratorModule.h"

ComponentRegister<AnomalyAcceleratorModule> AnomalyAcceleratorModule::
	s_reg("AnomalyAcceleratorModule");

AnomalyAcceleratorModule::AnomalyAcceleratorModule()
	: Component(ComponentType::AnomalyAcceleratorModule)
{
	cooldown = 0.0f;
}

void AnomalyAcceleratorModule::init( vector<ComponentData> p_initData )
{
}