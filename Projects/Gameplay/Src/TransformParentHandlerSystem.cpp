#include "TransformParentHandlerSystem.h"
#include "Transform.h"
#include "EntityParent.h"


EntityParentHandlerSystem::EntityParentHandlerSystem() : 
	EntitySystem( SystemType::TransformParentHandlerSystem, 2,
	ComponentType::ComponentTypeIdx::EntityParent,
	ComponentType::ComponentTypeIdx::Transform)
{

}


EntityParentHandlerSystem::~EntityParentHandlerSystem()
{
}

void EntityParentHandlerSystem::initialize()
{

}

void EntityParentHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for(unsigned int i=0; i<p_entities.size(); i++ )
	{
		EntityParent* nodeData = static_cast<EntityParent*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::EntityParent ) );

		Transform* transform = static_cast<Transform*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		Entity* parent = m_world->getEntity(nodeData->getParentEntityId());
		if (parent!=NULL)
		{
			Transform* parentTransform = static_cast<Transform*>(
				parent->getComponent( ComponentType::ComponentTypeIdx::Transform ) );
			// Set transform based on parent
			transform->setMatrix(nodeData->getLocalTransform()*parentTransform->getMatrix());
		}
		else // delete if no parent
		{
			m_world->deleteEntity(p_entities[i]);
		}


	}
}
