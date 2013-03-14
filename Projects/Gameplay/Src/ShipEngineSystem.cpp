#include "ShipEngineSystem.h"
#include "SoundComponent.h"
#include "ThrustComponent.h"
#include <AntTweakBarWrapper.h>
#include "Transform.h"

ShipEngineSystem::ShipEngineSystem() : EntitySystem(SystemType::ShipEngineSystem, 5,
													ComponentType::TAG_MyShip,
													ComponentType::Transform,
													ComponentType::MeshOffsetTransform,
													ComponentType::SoundComponent,
													ComponentType::ThrustComponent)
{
}

ShipEngineSystem::~ShipEngineSystem()
{

}

void ShipEngineSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(p_entities.size() > 0){
		for (unsigned int i = 0; i < p_entities.size(); i++){

			SoundComponent* soundComp = static_cast<SoundComponent*>
				(p_entities[i]->getComponent(ComponentType::SoundComponent));
			AudioHeader* idle = soundComp->getSoundHeaderByIndex(AudioHeader::AMBIENT, 
				m_shipIdleSoundIndex);

			ThrustComponent* thrust = static_cast<ThrustComponent*>
				(p_entities[i]->getComponent(ComponentType::ThrustComponent));

			float factor = thrust->m_thrustPower / (float)thrust->POWERCAP ;
			float soundVolume = factor;
			factor += 1.0f;
			idle->volume = soundVolume;
			idle->frequency = factor;
		}
	}
}

void ShipEngineSystem::inserted( Entity* p_entity ){
	SoundComponent* soundComp = static_cast<SoundComponent*>
		(p_entity->getComponent(ComponentType::SoundComponent));

	m_shipIdleSoundIndex = soundComp->getSoundIndexByName(
		AudioHeader::AMBIENT, "ShipEngineIdle");

	m_shipActiveSoundIndex = soundComp->getSoundIndexByName(
		AudioHeader::AMBIENT, "ShipEngineActive");
}

void ShipEngineSystem::removed( Entity* p_entity )
{
}
