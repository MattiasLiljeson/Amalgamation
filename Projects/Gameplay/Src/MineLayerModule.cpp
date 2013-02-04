#include "MineLayerModule.h"

ComponentRegister<MineLayerModule> MineLayerModule::s_reg("MineLayerModule");

MineLayerModule::MineLayerModule()
{
	m_componentType = ComponentType::MineLayerModule;
	m_cooldown = 0;
}

MineLayerModule::~MineLayerModule()
{

}

void MineLayerModule::init( vector<ComponentData> p_initData )
{
}