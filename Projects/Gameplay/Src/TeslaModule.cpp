#include "TeslaModule.h"

ComponentRegister<TeslaModule> TeslaModule::s_reg("TeslaModule");

TeslaModule::TeslaModule()
	: Component( ComponentType::TeslaModule )
{
}

TeslaModule::~TeslaModule()
{

}

void TeslaModule::init( vector<ComponentData> p_initData )
{
}