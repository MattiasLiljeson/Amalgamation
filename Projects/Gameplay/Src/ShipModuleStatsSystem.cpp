#include "ShipModuleStatsSystem.h"
#include "GameplayTags.h"
#include "ShipModule.h"
#include "NetworkSynced.h"
#include "TimerSystem.h"

unsigned int ShipModuleStatsSystem::m_currentStart = 0;

ShipModuleStatsSystem::ShipModuleStatsSystem( ModuleVisualEffectServerBufferSystem* p_effectBuffer )
	: EntitySystem(SystemType::ShipModuleStatsSystem, 1,
	ComponentType::ComponentTypeIdx::ShipModule)
{
	m_effectbuffer = p_effectBuffer;
}

ShipModuleStatsSystem::~ShipModuleStatsSystem()
{

}

void ShipModuleStatsSystem::initialize()
{

}

void ShipModuleStatsSystem::inserted( Entity* p_entity )
{

}

void ShipModuleStatsSystem::removed( Entity* p_entity )
{

}

void ShipModuleStatsSystem::processEntities( const vector<Entity*>& p_entities )
{

	// Module status are sent on each change in picking- and shipmodulecontroller,
	// but the client does not keep track
	// so send status updates every second in case player joined late(for example)
	// (Status effects works without this but might then glitch in these special cases!)
	if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::Every32Millisecond))
	{
		if (m_currentStart>=p_entities.size())
			m_currentStart=0;
		unsigned int end = min(p_entities.size(),m_currentStart+m_batchSz);
		// check a small batch every second
		// as it is not super important that visualization data is synced all the time
		// thus it is more important  to keep the footprint small
		for (unsigned int i = m_currentStart; i < end; i++)
		{
			ShipModule* module = static_cast<ShipModule*>(
				m_world->getComponentManager()->getComponent(p_entities[i],
				ComponentType::ShipModule));		

			Component* isShip = m_world->getComponentManager()->getComponent(
				p_entities[i],
				ComponentType::TAG_Ship);

			if (!isShip && module)
			{
				NetworkSynced* networkSynced = static_cast<NetworkSynced*>(
					p_entities[i]->getComponent(ComponentType::NetworkSynced));
				int id = networkSynced->getNetworkIdentity();

				// send stats updates
				if (module->isUnused())
					enableModuleUnusuedEffect(id);
				else
					disableModuleUnusuedEffect(id);

				updateModuleHealthEffect(id,module->m_health/module->getMaxHealth());
				updateModuleValueEffect(id,module->m_value/module->getMaxValue());
			}
		}
		m_currentStart+=m_batchSz;
	}
}

void ShipModuleStatsSystem::enableModuleUnusuedEffect( int p_moduleNetworkOwner )
{
	ModuleStatusEffectPacket fxPacket(ModuleStatusEffectPacket::UNUSEDMODULE_STATUS,
		ModuleStatusEffectPacket::ON,
		p_moduleNetworkOwner);

	m_effectbuffer->enqueueEffect(fxPacket);
}

void ShipModuleStatsSystem::disableModuleUnusuedEffect( int p_moduleNetworkOwner )
{
	ModuleStatusEffectPacket fxPacket(ModuleStatusEffectPacket::UNUSEDMODULE_STATUS,
		ModuleStatusEffectPacket::OFF,
		p_moduleNetworkOwner);

	m_effectbuffer->enqueueEffect(fxPacket);
}

void ShipModuleStatsSystem::updateModuleHealthEffect( int p_moduleNetworkOwner, 
													  float p_healthPercent )
{
	ModuleStatusEffectPacket fxPacket(ModuleStatusEffectPacket::HEALTH_STATUS,
		p_healthPercent,
		p_moduleNetworkOwner);

	m_effectbuffer->enqueueEffect(fxPacket);
}

void ShipModuleStatsSystem::updateModuleValueEffect( int p_moduleNetworkOwner, 
													 float p_valuePercent )
{
	ModuleStatusEffectPacket fxPacket(ModuleStatusEffectPacket::VALUE_STATUS,
		p_valuePercent,
		p_moduleNetworkOwner);

	m_effectbuffer->enqueueEffect(fxPacket);
}