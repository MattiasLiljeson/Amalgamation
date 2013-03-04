#include "DamageUpdaterSystem.h"
#include "DamageComponent.h"


DamageUpdaterSystem::DamageUpdaterSystem() 
	: EntitySystem( SystemType::DamageUpdaterSystem, 1,
	ComponentType::DamageComponent )
{
}


DamageUpdaterSystem::~DamageUpdaterSystem()
{
}

void DamageUpdaterSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int i=0; i<p_entities.size(); i++ ) {
		DamageComponent* dmgComp = static_cast<DamageComponent*>(
			p_entities[i]->getComponent( ComponentType::DamageComponent ) );
		dmgComp->pushFrameDamageToBuffer();
	}
}
