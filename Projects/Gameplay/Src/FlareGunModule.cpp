#include "FlareGunModule.h"

ComponentRegister<FlareGunModule> FlareGunModule::s_reg("FlareGunModule");

FlareGunModule::FlareGunModule()
	: Component( ComponentType::FlareGunModule )
{
}

FlareGunModule::~FlareGunModule()
{

}

void FlareGunModule::init( vector<ComponentData> p_initData )
{
}