#include "LoadMeshSystem.h"

#include "LoadMesh.h"
#include <ModelResource.h>
#include "Transform.h"
#include "EntityParent.h"
#include "ConnectionPointSet.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include "RenderInfo.h"
#include <Globals.h>

LoadMeshSystem::LoadMeshSystem( ) : 
	EntitySystem( SystemType::LoadMeshSystem, 1,
	ComponentType::ComponentTypeIdx::LoadMesh)
{

}


LoadMeshSystem::~LoadMeshSystem()
{
}

void LoadMeshSystem::initialize()
{

}

void LoadMeshSystem::processEntities( const vector<Entity*>& p_entities )
{
	float dt = m_world->getDelta();

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{
		Entity* entity = p_entities[i];
		//
		LoadMesh* jobInfo = static_cast<LoadMesh*>(
			entity->getComponent( ComponentType::ComponentTypeIdx::LoadMesh ) );

		// Load creation instructions
		vector<ModelResource*>* models = createModels( jobInfo->getFileName(), 
													   MODELPATH,
													   jobInfo->isPrimitive());
		// Root
		Transform* rootTransformData=NULL;
		setRootData(entity,(*models)[0],rootTransformData);
		// Children
		if (models->size()>1)
			createChildrenEntities(models,entity);

		// remove init data and update
		p_entities[i]->removeComponent(ComponentType::LoadMesh);
		p_entities[i]->applyComponentChanges();
	}
}



void LoadMeshSystem::setRootData( Entity* p_entity, ModelResource* p_modelResource, 
									   Transform* p_outTransform )
{
	Component* t = p_entity->getComponent( ComponentType::ComponentTypeIdx::Transform );
	if (t!=NULL)
		p_outTransform = static_cast<Transform*>(t);

	int rootId = p_entity->getIndex(); // store the id

	Component* component = NULL;

	// Mesh, renderinfo
	int meshId = p_modelResource->meshId;
	if (meshId!=-1)
	{
		component = new RenderInfo( meshId );
		p_entity->addComponent( ComponentType::RenderInfo, component );
	}


	// Connection points
	if (!p_modelResource->connectionPoints.m_collection.empty())
	{
		Component* component = new ConnectionPointSet( p_modelResource->connectionPoints.m_collection );
		p_entity->addComponent( ComponentType::ConnectionPointSet, component );
	}

	// Handle particles here
	if (!p_modelResource->particleSystems.m_collection.empty())
	{

	}

	// Transform
	if (p_outTransform==NULL) // only add transform for first, if none already exist
	{
		p_outTransform = new Transform( p_modelResource->transform );
		p_entity->addComponent( ComponentType::Transform, p_outTransform );
	}
}



void LoadMeshSystem::createChildrenEntities( vector<ModelResource*>* p_modelResources, 
												  Entity* p_rootEntity)
{
	Component* t = NULL;
	Transform* rootTransform = NULL;
	BodyInitData* rootRigidBody = NULL;
	PhysicsBody* rootPhysicsBody = NULL;
	int rootId = p_rootEntity->getIndex();

	// parent transform
	t = p_rootEntity->getComponent( ComponentType::ComponentTypeIdx::Transform );
	if (t!=NULL)
		rootTransform = static_cast<Transform*>(t);

	// parent rigidbody data
	t = p_rootEntity->getComponent( ComponentType::ComponentTypeIdx::BodyInitData );
	if (t!=NULL)
		rootRigidBody = static_cast<BodyInitData*>(t);

	// parent rigidbody
	t = p_rootEntity->getComponent( ComponentType::ComponentTypeIdx::PhysicsBody );
	if (t!=NULL)
		rootPhysicsBody = static_cast<PhysicsBody*>(t);

	//
	ModelResource* modelResource = NULL;
	Entity* entity = NULL;
	Component* component;
	for (unsigned int i=1;i<p_modelResources->size();i++)
	{
		modelResource = (*p_modelResources)[i];		// fetch instruction
		entity = m_world->createEntity();			// create entity

		// Mesh, renderinfo
		int meshId = modelResource->meshId;
		if (meshId!=-1)
		{
			component = new RenderInfo( meshId );
			entity->addComponent( ComponentType::RenderInfo, component );
		}

		// Transform
		AglMatrix baseTransform = AglMatrix::identityMatrix();
		// if (rootTransform) baseTransform = rootTransform->getMatrix();
		baseTransform = modelResource->transform * baseTransform;

		component = new Transform( baseTransform );
		entity->addComponent( ComponentType::Transform, component );

		// Connection points
		if (!modelResource->connectionPoints.m_collection.empty())
		{
			component = new ConnectionPointSet( modelResource->connectionPoints.m_collection );
			entity->addComponent( ComponentType::ConnectionPointSet, component );
		}

		// Handle particles here
		if (!modelResource->particleSystems.m_collection.empty())
		{

		}

		// Collision
		if (rootRigidBody)
		{
			PhysicsBody* pb = new PhysicsBody();
			if (rootRigidBody->m_compound)
				pb->setParentId(rootPhysicsBody->m_id);
			entity->addComponent( ComponentType::PhysicsBody, 
				pb );


			BodyInitData* b = new BodyInitData(AglVector3(30, 0, 0),
				AglQuaternion::identity(),
				AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
				AglVector3(0, 0, 0), 0);
			// Copy from parent
			b->m_position		= baseTransform.GetTranslation();
			b->m_orientation	= baseTransform.GetRotation();
			b->m_scale			= baseTransform.GetScale();
			b->m_velocity			= rootRigidBody->m_velocity;
			b->m_angularVelocity	= rootRigidBody->m_angularVelocity;
			b->m_collisionEnabled	= rootRigidBody->m_collisionEnabled;
			b->m_compound			= rootRigidBody->m_compound;
			b->m_impulseEnabled		= rootRigidBody->m_impulseEnabled;

			b->m_type = rootRigidBody->m_type;

			entity->addComponent( ComponentType::BodyInitData, b);
		}

		// Hierarchy
		component = new EntityParent( rootId, modelResource->transform );
		entity->addComponent( ComponentType::EntityParent, component );

		// finished
		m_world->addEntity(entity);		
	}
}
