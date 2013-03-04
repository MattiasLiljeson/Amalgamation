#define FORCE_VS_DBG_OUTPUT

#include "LevelHandlerSystem.h"
#include "LevelPieceRoot.h"
#include "Transform.h"
#include "SpawnPointSystem.h"
#include "LevelGenSystem.h"

LevelHandlerSystem::LevelHandlerSystem()
	: EntitySystem(SystemType::LevelHandlerSystem, 2, ComponentType::LevelPieceRoot, ComponentType::Transform)
{
	m_hasLevel = false;
}

LevelHandlerSystem::~LevelHandlerSystem()
{

}

void LevelHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{

}

void LevelHandlerSystem::inserted( Entity* p_entity )
{
	auto pieceRoot = static_cast<LevelPieceRoot*>(
		p_entity->getComponent(ComponentType::LevelPieceRoot));

	auto levelGen = static_cast<LevelGenSystem*>(
		m_world->getSystem(SystemType::LevelGenSystem));

	if (levelGen && pieceRoot->pieceId == levelGen->getGeneratedPiecesCount() - 1)
	{
		m_hasLevel = true;
		DEBUGPRINT(("Level is now properly generated and loaded.\n"));
	}
}

void LevelHandlerSystem::destroyLevel()
{
	vector<Entity*> entities = getActiveEntities();
	for (int i = 0; i < entities.size(); i++)
	{
		m_world->deleteEntity(entities[i]);
	}
	m_hasLevel = false;
}

bool LevelHandlerSystem::hasLevel() const
{
	return m_hasLevel;
}
