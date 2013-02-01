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

	ModelResource* modelResource = p_modelResource;
	Entity* entity = p_entity;

	// Mesh, renderinfo
	setUpRenderInfo(entity,modelResource);

	// Connection points
	setUpConnectionPoints(entity,modelResource);

	// Handle particles here
	setUpParticles(entity,modelResource);



	BodyInitData* initData = static_cast<BodyInitData*>(p_entity->getComponent(ComponentType::BodyInitData));
	if (initData)
	{
		if (initData->m_type == BodyInitData::BOXFROMMESHOBB)
		{
			initData->m_modelResource = p_modelResource; 
		}
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
		setUpRenderInfo(entity,modelResource);

		// Transform
		AglMatrix baseTransform = AglMatrix::identityMatrix();
		baseTransform = modelResource->transform * baseTransform;
		component = new Transform( baseTransform );
		entity->addComponent( ComponentType::Transform, component );

		// Connection points
		setUpConnectionPoints(entity,modelResource);

		// Particles
		setUpParticles(entity,modelResource);

		// Collision
		setUpChildCollision(entity,modelResource,
			rootRigidBody,rootPhysicsBody,baseTransform);

		// Hierarchy
		component = new EntityParent( rootId, modelResource->transform );
		entity->addComponent( ComponentType::EntityParent, component );

		// finished
		m_world->addEntity(entity);		
	}
}


void LoadMeshSystem::setUpRootCollision( Entity* p_entity, ModelResource* p_modelResource )
{

}

void LoadMeshSystem::setUpRenderInfo( Entity* p_entity, ModelResource* p_modelResource )
{
	int meshId = p_modelResource->meshId;
	if (meshId!=-1)
	{
		Component* component = new RenderInfo( meshId );
		p_entity->addComponent( ComponentType::RenderInfo, component );
	}
}

void LoadMeshSystem::setUpConnectionPoints( Entity* p_entity, 
										   ModelResource* p_modelResource )
{
	if (!p_modelResource->connectionPoints.m_collection.empty())
	{
		Component* component = new ConnectionPointSet( p_modelResource->connectionPoints.m_collection );
		p_entity->addComponent( ComponentType::ConnectionPointSet, component );
	}
}

void LoadMeshSystem::setUpParticles( Entity* p_entity, ModelResource* p_modelResource )
{
	// NOT IMPLEMENTED
}