#include "MineLayerModule.h"

ComponentRegister<MineLayerModule> MineLayerModule::s_reg("MineLayerModule");

MineLayerModule::MineLayerModule()
	: Component( ComponentType::MineLayerModule )
{
	m_cooldown = 0;
	m_timeSinceMineSpawnStart = 0;
}

MineLayerModule::~MineLayerModule()
{

}

void MineLayerModule::init( vector<ComponentData> p_initData )
{
}