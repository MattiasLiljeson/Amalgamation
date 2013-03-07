#include "ShipEngineSystem.h"

ShipEngineSystem::ShipEngineSystem() : EntitySystem(SystemType::ShipEngineSystem, 4,
													ComponentType::TAG_Ship,
													ComponentType::Transform,
													ComponentType::MeshOffsetTransform,
													ComponentType::SoundComponent)
{
	x = 10;
}

ShipEngineSystem::~ShipEngineSystem()
{

}

void ShipEngineSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(p_entities.size() > 0){
		x = p_entities.size();
	}
}
