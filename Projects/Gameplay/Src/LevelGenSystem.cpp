#include "LevelGenSystem.h"
#include "Transform.h"
#include <AglVector3.h>

LevelGenSystem::LevelGenSystem() : EntitySystem(SystemType::LevelGenSystem)
{

}

LevelGenSystem::~LevelGenSystem()
{

}

void LevelGenSystem::initialize()
{
	generateLevelPieces(4);
}

void LevelGenSystem::generateLevelPieces( int p_depth )
{
	// Create the first entity piece
	Entity* entity = m_world->createEntity();
	// Create root transform
	Transform* transform = new Transform(AglVector3::zero(), AglQuaternion::identity(),
										AglVector3::one());

	entity->addComponent(ComponentType::Transform, transform);

	

	m_world->addEntity(entity);

}
