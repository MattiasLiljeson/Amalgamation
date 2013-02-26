#include "AnomalyAcceleratorModule.h"

ComponentRegister<AnomalyAcceleratorModule> AnomalyAcceleratorModule::
	s_reg("AnomalyAcceleratorModule");

AnomalyAcceleratorModule::AnomalyAcceleratorModule()
	: Component(ComponentType::AnomalyAcceleratorModule)
{
}

void AnomalyAcceleratorModule::init( vector<ComponentData> p_initData )
{
}