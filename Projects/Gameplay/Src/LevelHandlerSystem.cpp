#include "LevelHandlerSystem.h"
#include "LevelPieceRoot.h"
#include "Transform.h"
#include "SpawnPointSystem.h"

LevelHandlerSystem::LevelHandlerSystem()
	: EntitySystem(SystemType::LevelHandlerSystem, 2, ComponentType::LevelPieceRoot, ComponentType::Transform)
{

}

LevelHandlerSystem::~LevelHandlerSystem()
{

}

void LevelHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{

}

void LevelHandlerSystem::inserted( Entity* p_entity )
{
	// No longer required right now.

	//auto transform = static_cast<Transform*>(
	//	p_entity->getComponent(ComponentType::Transform));
	//auto pieceRoot = static_cast<LevelPieceRoot*>(
	//	p_entity->getComponent(ComponentType::LevelPieceRoot));

	//// Init the translation and rotation here. Uncertain if scale should ever be applied.
	//transform->setTranslation( pieceRoot->initTransformMatrix.GetTranslation() );
	//transform->setRotation( pieceRoot->initTransformMatrix.GetRotation() );
}

void LevelHandlerSystem::destroyLevel()
{
	vector<Entity*> entities = getActiveEntities();
	for (int i = 0; i < entities.size(); i++)
	{
		m_world->deleteEntity(entities[i]);
	}
}
