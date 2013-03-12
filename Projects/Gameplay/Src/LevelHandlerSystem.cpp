#define FORCE_VS_DBG_OUTPUT

#include "LevelHandlerSystem.h"
#include "LevelPieceRoot.h"
#include "Transform.h"
#include "SpawnPointSystem.h"
#include "LevelGenSystem.h"
#include <OutputLogger.h>

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

	m_pieceRoots.push_back(pieceRoot);

	if (levelGen && getActiveEntities().size() == levelGen->getLevelPieceRootCount())
	{
		m_hasLevel = true;
		m_world->getOutputLogger()
			->write("Level is now properly generated and loaded.\n");
	}
}

void LevelHandlerSystem::destroyLevel()
{
	vector<Entity*> entities = getActiveEntities();
	for (int i = 0; i < entities.size(); i++)
	{
		m_world->deleteEntity(entities[i]);
	}
	m_pieceRoots.clear();
	m_hasLevel = false;
}

bool LevelHandlerSystem::hasLevel() const
{
	return m_hasLevel;
}
