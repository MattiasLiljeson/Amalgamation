#include "ShieldPlatingSystem.h"
#include "ShieldPlate.h"
#include "Transform.h"
#include "EntityParent.h"

ShieldPlatingSystem::ShieldPlatingSystem()
	: EntitySystem( SystemType::ShieldPlatingSystem, 3, ComponentType::Transform,
	ComponentType::ShieldPlate, ComponentType::EntityParent)
{
}

void ShieldPlatingSystem::inserted( Entity* p_entity )
{
	ShieldPlate* plate = static_cast<ShieldPlate*>(p_entity->getComponent(
		ComponentType::ShieldPlate));
	EntityParent* child = static_cast<EntityParent*>(p_entity->getComponent(
		ComponentType::EntityParent));
	plate->spawnTransform = child->getLocalTransform();
}

void ShieldPlatingSystem::processEntities( const vector<Entity*>& p_entities )
{
	for(unsigned int i=0; i<p_entities.size(); i++)
	{
		ShieldPlate* plate = static_cast<ShieldPlate*>(p_entities[i]->getComponent(
			ComponentType::ShieldPlate));
		EntityParent* child = static_cast<EntityParent*>(p_entities[i]->getComponent(
			ComponentType::EntityParent));
		float deltaScale = m_world->getDelta() * 0.5f;
		plate->scale -= deltaScale;
		if(plate->scale < 0.0f)
		{
			child->setLocalTransform(plate->spawnTransform); // Respawn.
			plate->scale = plate->scaleSeed;
		}
		AglVector3 positionComponent;
		AglQuaternion quarternionComponent;
		AglVector3 scaleComponent;
		AglMatrix::matrixToComponents(child->getLocalTransform(),
			scaleComponent, quarternionComponent, positionComponent);
		scaleComponent.x = plate->scale;
		scaleComponent.y = plate->scale;
		scaleComponent.z = plate->scale;
		AglMatrix rescaledLocalTransform;
		AglMatrix::componentsToMatrix(rescaledLocalTransform,
			scaleComponent, quarternionComponent, positionComponent);
		child->setLocalTransform(rescaledLocalTransform);
	}
}