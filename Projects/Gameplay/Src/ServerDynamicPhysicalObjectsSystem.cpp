#include "ServerDynamicPhysicalObjectsSystem.h"

ServerDynamicPhysicalObjectsSystem::ServerDynamicPhysicalObjectsSystem()
	: EntitySystem(SystemType::ServerDynamicPhysicalObjectsSystem, 3,
	ComponentType::NetworkSynced, ComponentType::Transform, ComponentType::PhysicsBody)
{
}