#include "LoadMeshSystem.h"

#include "LoadMesh.h"
#include <ModelResource.h>
#include "Transform.h"
#include "EntityParent.h"
#include "ConnectionPointSet.h"
#include "LightsComponent.h"
#include "BodyInitData.h"
#include "ConnectionPointSet.h"
#include "EntityParent.h"
#include "LoadMesh.h"
#include "MeshOffsetTransform.h"
#include "ParticleSystemsComponent.h"
#include "PhysicsBody.h"
#include "RenderInfo.h"
#include "Transform.h"
#include <Globals.h>
#include <ModelResource.h>
#include "SpawnPointSet.h"

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
		if (models->size()==1)
			setUpRootCollision(entity, (*models)[0]);

		// Children
		if (models->size()>1)
			createChildrenEntities(models,entity);

		// remove init data and update
		p_entities[i]->removeComponent(ComponentType::LoadMesh);
		p_entities[i]->applyComponentChanges();
	}
}

//void LoadMeshSystem::inserted( Entity* p_entity )
//{
//	LoadMesh* jobInfo = static_cast<LoadMesh*>(
//		p_entity->getComponent( ComponentType::ComponentTypeIdx::LoadMesh ) );
//
//	// Load creation instructions
//	vector<ModelResource*>* models = createModels( jobInfo->getFileName(), 
//		MODELPATH,
//		jobInfo->isPrimitive());
//	// Root
//	Transform* rootTransformData=NULL;
//	setRootData(p_entity,(*models)[0],rootTransformData);
//	// Children
//	if (models->size()>1)
//		createChildrenEntities(models,p_entity);
//
//	// remove init data and update
//	p_entity->removeComponent(ComponentType::LoadMesh);
//	p_entity->applyComponentChanges();
//}


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

	// Lights
	setUpLights(entity,modelResource);

	// Spawn points
	setUpSpawnPoints(entity,modelResource);

	// Handle particles here
	setUpParticles(entity,modelResource);

	//Handle animations
	setUpAnimation(entity, modelResource);

	//Should not be here - ONLY RELEVANT FOR SHIP
	BodyInitData* initData = static_cast<BodyInitData*>(p_entity->getComponent(ComponentType::BodyInitData));
	if (initData)
	{
		if (initData->m_type == BodyInitData::BOXFROMMESHOBB && !p_entity->getComponent(ComponentType::MeshOffsetTransform))
		{
			initData->m_modelResource = p_modelResource; 
			p_entity->addComponent(ComponentType::MeshOffsetTransform, new MeshOffsetTransform(p_modelResource->meshHeader.transform));
		}

		//Should not be here but is common with body init data right now

	}
	else
		p_entity->addComponent(ComponentType::MeshOffsetTransform, new MeshOffsetTransform(p_modelResource->meshHeader.transform));

	if (p_modelResource->connectionPoints.m_collection.size() > 0)
	{
		ConnectionPointSet* connectionPointSet = static_cast<ConnectionPointSet*>(p_entity->getComponent(ComponentType::ConnectionPointSet));
		AglMatrix inv = p_modelResource->meshHeader.transform.inverse();
		for (unsigned int i = 0; i < connectionPointSet->m_connectionPoints.size(); i++)
		{
			//This inverse is performed to bring the connection point from world space
			//to local space of the mesh. This should not really be done if the mesh
			//is already parent to the transform. Make check for this later.
			connectionPointSet->m_connectionPoints[i].cpTransform *= inv;
		}
	}

	//END should not be here

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
		if (rootTransform)
			baseTransform = rootTransform->getMatrix();

		baseTransform = modelResource->transform * baseTransform;
		component = new Transform( baseTransform );
		entity->addComponent( ComponentType::Transform, component );

		// Connection points
		setUpConnectionPoints(entity,modelResource);

		// Lights
		setUpLights(entity,modelResource);

		// Spawn points
		setUpSpawnPoints(entity,modelResource);

		// Particles
		setUpParticles(entity,modelResource);

		// Collision
		setUpChildCollision(entity,modelResource,
			rootRigidBody,rootPhysicsBody,baseTransform);

		//Animation
		setUpAnimation(entity, modelResource);

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
		ConnectionPointSet* component = new ConnectionPointSet();
		for (unsigned int i = 0; i < p_modelResource->connectionPoints.m_collection.size(); i++)
		{
			ConnectionPoint cp(p_modelResource->connectionPoints.m_collection[i]);
			component->m_connectionPoints.push_back(cp);
		}
		p_entity->addComponent( ComponentType::ConnectionPointSet, component );
	}
}


void LoadMeshSystem::setUpSpawnPoints( Entity* p_entity, ModelResource* p_modelResource )
{
	if (!p_modelResource->spawnPoints.m_collection.empty())
	{
		Component* component = new SpawnPointSet( p_modelResource->spawnPoints.m_collection );
		p_entity->addComponent( ComponentType::SpawnPointSet, component );
	}
}


void LoadMeshSystem::setUpLights( Entity* p_entity, ModelResource* p_modelResource )
{
	vector<LightCreationData>* lights= &(p_modelResource->lightCollection.m_collection);
	if (!lights->empty())
	{
		LightsComponent* component = new LightsComponent();
		for (unsigned int i=0;i<lights->size();i++)
		{
			// This'll be fun		
			LightCreationData* source = &(*lights)[i];
			Light light;
			TransformComponents transformHelper;
			transformHelper.scale = AglVector3(source->range,source->range,source->range);
			transformHelper.rotation = source->transform.GetRotation();
			transformHelper.translation = source->transform.GetTranslation();
			light.offsetMat = transformHelper.toMatrix();
			AglVector3 forward = source->transform.GetForward();
			light.instanceData.lightDir[0] = forward.x;
			light.instanceData.lightDir[1] = forward.y;
			light.instanceData.lightDir[2] = forward.z;
			light.instanceData.color[0] = source->diffuse.x;
			light.instanceData.color[1] = source->diffuse.y;
			light.instanceData.color[2] = source->diffuse.z;
			//light.instanceData.specular[0] = source->specular.x;
			//light.instanceData.specular[1] = source->specular.y;
			//light.instanceData.specular[2] = source->specular.z;
			//light.instanceData.specular[3] = source->gloss;
			//light.instanceData.ambient[0] = source->ambient.x;
			//light.instanceData.ambient[1] = source->ambient.y;
			//light.instanceData.ambient[2] = source->ambient.z;
			if (source->type==LightCreationData::POINT)
				light.instanceData.type = LightTypes::E_LightTypes_POINT;
			else if (source->type==LightCreationData::SPOT)
				light.instanceData.type = LightTypes::E_LightTypes_SPOT;
			else
				light.instanceData.type = LightTypes::E_LightTypes_DIRECTIONAL;
			light.instanceData.range = source->range;
			light.instanceData.attenuation[0] = source->attenuation.x;
			light.instanceData.attenuation[1] = source->attenuation.y;
			light.instanceData.attenuation[2] = source->attenuation.z;
			light.instanceData.lightEnergy = source->power;
			component->addLight(light);
		}
		p_entity->addComponent( ComponentType::LightsComponent, component );
	}
}

void LoadMeshSystem::setUpParticles( Entity* p_entity, ModelResource* p_modelResource )
{
	if (!p_modelResource->particleSystems.m_collection.empty())
	{
		ParticleSystemsComponent* particleComp = static_cast<ParticleSystemsComponent*>(
			p_entity->getComponent( ComponentType::ParticleSystemsComponent ) );

		if( particleComp == NULL ) {
			particleComp = new ParticleSystemsComponent();
			p_entity->addComponent( particleComp );
		}

		particleComp->addParticleSystemInstructions( p_modelResource->particleSystems );
	}
}

