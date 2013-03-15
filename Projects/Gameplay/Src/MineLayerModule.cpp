#include "MineLayerModule.h"

ComponentRegister<MineLayerModule> MineLayerModule::s_reg("MineLayerModule");

MineLayerModule::MineLayerModule()
	: Component( ComponentType::MineLayerModule )
{
	m_currentMine = -1;
	m_mineAge = 0;
	m_timeSinceSpawnStarted = 0;
	m_cooldown = 0;
}

MineLayerModule::~MineLayerModule()
{

}

void MineLayerModule::init( vector<ComponentData> p_initData )
{
}