#include "AddToParentSystem.h"
#include "EntityParent.h"
#include "LevelPieceRoot.h"
#include "BoundingSphere.h"
#include "MeshOffsetTransform.h"
#include "Transform.h"

AddToParentSystem::AddToParentSystem() : EntitySystem(SystemType::AddToParentSystem, 1, ComponentType::TAG_AddToParent)
{

}
AddToParentSystem::~AddToParentSystem()
{

}
void AddToParentSystem::initialize()
{

}

void AddToParentSystem::processEntities( const vector<Entity*>& p_entities )
{
	for (unsigned int i = 0; i < p_entities.size(); i++)
	{
		EntityParent* parentRef = static_cast<EntityParent*>
			(p_entities[i]->getComponent(ComponentType::EntityParent));

		if (parentRef)
		{
			Entity* parent = m_world->getEntity(parentRef->getParentEntityId());
			
			LevelPieceRoot* piece = static_cast<LevelPieceRoot*>
				(parent->getComponent(ComponentType::LevelPieceRoot));

			if (piece)
			{
				AglBoundingSphere toAdd = getBoundingSphereWorld(p_entities[i]);
				if (piece->boundingSphere.radius > 0)
				{
					piece->boundingSphere = 
						AglBoundingSphere::mergeSpheres(toAdd, piece->boundingSphere);
				}
				else
				{
					piece->boundingSphere = toAdd;
				}
				p_entities[i]->removeComponent(ComponentType::TAG_AddToParent);
				p_entities[i]->applyComponentChanges();
			}
		}
	}
}

AglBoundingSphere AddToParentSystem::getBoundingSphereWorld(Entity* p_entity)
{
	Transform* transform = static_cast<Transform*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

	BoundingSphere* bs = static_cast<BoundingSphere*>(
		p_entity->getComponent( ComponentType::ComponentTypeIdx::BoundingSphere ) );

	AglBoundingSphere sphere = bs->sphere;
	sphere.position.transform(transform->getMatrix());
	AglVector3 scale = transform->getScale();

	sphere.radius *= max(scale.x, max(scale.y, scale.z));

	return sphere;
}