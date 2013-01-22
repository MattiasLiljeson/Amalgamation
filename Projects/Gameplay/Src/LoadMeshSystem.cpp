#include "LoadMeshSystem.h"

#include "GraphicsBackendSystem.h"
#include "LoadMeshJobComponent.h"
#include <ModelResource.h>
#include <GraphicsWrapper.h>
#include "Transform.h"
#include "RenderInfo.h"
#include "EntityParent.h"
#include "ConnectionPointSet.h"

LoadMeshSystem::LoadMeshSystem( GraphicsBackendSystem* p_gfxBackend ) : 
	EntitySystem( SystemType::LoadMeshSystem, 1,
	ComponentType::ComponentTypeIdx::LoadMeshJobComponent)
{
	m_gfxBackend = p_gfxBackend;
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
	GraphicsWrapper* gfxWrapper = m_gfxBackend->getGfxWrapper();

	for(unsigned int i=0; i<p_entities.size(); i++ )
	{
		Entity* entity = p_entities[i];
		//
		LoadMeshJobComponent* jobInfo = static_cast<LoadMeshJobComponent*>(
			entity->getComponent( ComponentType::ComponentTypeIdx::LoadMeshJobComponent ) );

		// Load creation instructions
		vector<ModelResource*>* models= gfxWrapper->createModelsFromFile( jobInfo->getFileName(), 
																		  &MODELPATH);
		// Root
		Transform* rootTransformData=NULL;
		int rootId = setRootData(entity,(*models)[0],rootTransformData);
		// Children
		createChildrenEntities(models,rootId,rootTransformData);

		// remove init data and update
		p_entities[i]->removeComponent(ComponentType::LoadMeshJobComponent);
		p_entities[i]->applyComponentChanges();
	}
}



int LoadMeshSystem::setRootData( Entity* p_entity, ModelResource* p_modelResource, 
								 Transform* p_outTransform )
{
	Component* t = p_entity->getComponent( ComponentType::ComponentTypeIdx::Transform );
	if (t!=NULL)
		p_outTransform = static_cast<Transform*>(t);

	int rootId = p_entity->getIndex(); // store the id

	Component* component = NULL;

	// Connection points
	if (!p_modelResource->connectionPoints.m_collection.empty())
	{
		Component* component = new ConnectionPointSet( p_modelResource->connectionPoints.m_collection );
		p_entity->addComponent( ComponentType::ConnectionPointSet, component );
	}
	
	// Handle particles here

	// Transform
	if (p_outTransform==NULL) // only add transform for first, if none already exist
	{
		p_outTransform = new Transform( p_modelResource->transform );
		p_entity->addComponent( ComponentType::Transform, p_outTransform );
	}
}



void LoadMeshSystem::createChildrenEntities( vector<ModelResource*>* p_modelResources, 
												int p_rootId, Transform* p_rootTransform)
{
	ModelResource* modelResource = NULL;
	Entity* entity = NULL;
	Component* component;
	for (int i=1;i<p_modelResources->size();i++)
	{
		modelResource = (*p_modelResources)[i];		// fetch instruction
		entity = m_world->createEntity();	// create entity

		// mesh
		int meshId = modelResource->meshId;
		component = new RenderInfo( meshId );
		entity->addComponent( ComponentType::RenderInfo, component );

		// transform
		AglMatrix baseTransform = AglMatrix::identityMatrix();
		if (p_rootTransform) baseTransform = p_rootTransform->getMatrix();

		component = new Transform( modelResource->transform * baseTransform );
		entity->addComponent( ComponentType::Transform, component );

		// connection points
		if (!modelResource->connectionPoints.m_collection.empty())
		{
			component = new ConnectionPointSet( modelResource->connectionPoints.m_collection );
			entity->addComponent( ComponentType::ConnectionPointSet, component );
		}

		// Handle particles here

		// hierarchy
		component = new EntityParent( p_rootId, modelResource->transform );
		entity->addComponent( ComponentType::EntityParent, component );

		// finished
		m_world->addEntity(entity);		
	}
}