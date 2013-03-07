#include "PortalCullingSystem.h"
#include "LevelPieceRoot.h"

PortalCullingSystem::PortalCullingSystem()
	: EntitySystem( SystemType::PortalCullingSystem, 1,
	ComponentType::ComponentTypeIdx::LevelPieceRoot )
{	
}

PortalCullingSystem::~PortalCullingSystem()
{

}

void PortalCullingSystem::initialize()
{
}

void PortalCullingSystem::processEntities( const vector<Entity*>& p_entities )
{
	for( unsigned int i=0; i<p_entities.size(); i++ )
	{
		LevelPieceRoot* root = static_cast<LevelPieceRoot*>(p_entities[i]->getComponent(ComponentType::LevelPieceRoot));
		if (root->boundingSphere.radius > 0)
			return;
	}
}