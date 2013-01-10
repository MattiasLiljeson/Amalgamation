#include "ShieldModuleControllerSystem.h"
#include "Transform.h"
#include "RenderInfo.h"
#include "GraphicsBackendSystem.h"
#include <AglMatrix.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "..\..\Input\Src\Control.h"
#include "PhysicsBody.h"
#include "BodyInitData.h"
#include "PhysicsSystem.h"

ShieldModuleControllerSystem::ShieldModuleControllerSystem()
	: EntitySystem(SystemType::ShieldModuleControllerSystem, 1, ComponentType::ShieldModule)
{
}


ShieldModuleControllerSystem::~ShieldModuleControllerSystem()
{
}

void ShieldModuleControllerSystem::initialize()
{
}

void ShieldModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
	}
}