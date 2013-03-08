#include "ClientModuleCounterSystem.h"
#include <AntTweakBarWrapper.h>

ClientModuleCounterSystem::ClientModuleCounterSystem()
	: EntitySystem(SystemType::ClientModuleCounterSystem, 3, ComponentType::Transform,
	ComponentType::NetworkSynced, ComponentType::ShipModule)
{
	m_numberOfModules = 0;
}

void ClientModuleCounterSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_numberOfModules = p_entities.size();
}

void ClientModuleCounterSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::NETWORK,
		"num modules", TwType::TW_TYPE_UINT32, &m_numberOfModules, "");
}