#include "MineLayerModuleControllerSystem.h"
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
#include "ShipModule.h"

MineLayerModuleControllerSystem::MineLayerModuleControllerSystem()
	: EntitySystem(SystemType::MinigunModuleControllerSystem, 1, ComponentType::MineLayerModule)
{
}


MineLayerModuleControllerSystem::~MineLayerModuleControllerSystem()
{
}

void MineLayerModuleControllerSystem::initialize()
{
}

void MineLayerModuleControllerSystem::processEntities(const vector<Entity*>& p_entities)
{
	float dt = m_world->getDelta();

	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		ShipModule* module = static_cast<ShipModule*>(
			m_world->getComponentManager()->getComponent(p_entities[i],
			ComponentType::getTypeFor(ComponentType::ShipModule)));
	}
}