#include "EmpModule.h"

ComponentRegister<EmpModule> EmpModule::s_reg("EmpModule");

EmpModule::EmpModule()
	: Component( ComponentType::EmpModule )
{
}

EmpModule::~EmpModule()
{

}

void EmpModule::init( vector<ComponentData> p_initData )
{
}