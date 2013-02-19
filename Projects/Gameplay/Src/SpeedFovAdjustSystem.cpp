#include "SpeedFovAdjustSystem.h"


SpeedFovAdjustSystem::SpeedFovAdjustSystem() : 
	EntitySystem( SystemType::InterpolationSystem, 2,
	ComponentType::TAG_MainCamera, ComponentType::Transform )
{
}


SpeedFovAdjustSystem::~SpeedFovAdjustSystem()
{
}

void SpeedFovAdjustSystem::processEntities( const vector<Entity*>& p_entities )
{

}
