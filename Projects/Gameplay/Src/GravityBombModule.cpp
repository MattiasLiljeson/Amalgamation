#include "GravityBombModule.h"

ComponentRegister<GravityBombModule> GravityBombModule::s_reg("GravityBombModule");

GravityBombModule::GravityBombModule()
	: Component( ComponentType::GravityBombModule )
{
}

GravityBombModule::~GravityBombModule()
{

}

void GravityBombModule::init( vector<ComponentData> p_initData )
{
}