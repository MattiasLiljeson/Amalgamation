#include "TransformParentHandlerSystem.h"
#include "Transform.h"
#include "EntityParent.h"
#include <DebugUtil.h>


EntityParentHandlerSystem::EntityParentHandlerSystem() : 
	EntitySystem( SystemType::TransformParentHandlerSystem, 2,
	ComponentType::ComponentTypeIdx::EntityParent,
	ComponentType::ComponentTypeIdx::Transform)
{

}


EntityParentHandlerSystem::~EntityParentHandlerSystem()
{
}

void EntityParentHandlerSystem::inserted( Entity* p_entity )
{
	// Store debug information
	EntityParent* parent = static_cast<EntityParent*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::EntityParent ) );
	m_entityHierarchy[p_entity->getIndex()] = parent->getParentEntityId();
}

void EntityParentHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();
	for(unsigned int i=0; i<p_entities.size(); i++ )
	{
		EntityParent* nodeData = static_cast<EntityParent*>(
			p_entities[i]->getComponent( ComponentType::ComponentTypeIdx::EntityParent ) );

		if(!checkDebugInformation(p_entities[i]->getIndex(), nodeData->getParentEntityId()))
		{
			DEBUGWARNING(( "Warning: Déjà vu, there was a change in the Matrix.\n" ));
		}

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

bool EntityParentHandlerSystem::checkDebugInformation( int p_child, int p_parent )
{
	return m_entityHierarchy[p_child] == p_parent;
}
