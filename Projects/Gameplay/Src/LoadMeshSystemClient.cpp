#include "LoadMeshSystemClient.h"

#include "BodyInitData.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "ConnectionPointSet.h"
#include "EntityParent.h"
#include "GraphicsBackendSystem.h"
#include "LoadMesh.h"
#include "MeshOffsetTransform.h"
#include "ParticleSystemsComponent.h"
#include "PhysicsBody.h"
#include "RenderInfo.h"
#include "SkeletalAnimation.h"
#include "Transform.h"
#include <AglSkeletonMapping.h>
#include <GraphicsWrapper.h>
#include <ModelResource.h>

LoadMeshSystemClient::LoadMeshSystemClient( GraphicsBackendSystem* p_gfxBackend ) : 
	LoadMeshSystem()
{
	m_gfxBackend = p_gfxBackend;
}


LoadMeshSystemClient::~LoadMeshSystemClient()
{
}

void LoadMeshSystemClient::initialize()
{

}

vector<ModelResource*>* LoadMeshSystemClient::createModels( const string& p_filename, const string& p_filePath, bool p_isPrimitive )
{
	return m_gfxBackend->getGfxWrapper()->createModelsFromFile(p_filename,&p_filePath,p_isPrimitive);
}

void LoadMeshSystemClient::setUpRootCollision(Entity* p_entity, ModelResource* p_modelResource)
{
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(p_entity->getComponent(ComponentType::MeshOffsetTransform));
	if (!offset)
	{
		offset = new MeshOffsetTransform(p_modelResource->meshHeader.transform);
		p_entity->addComponent(ComponentType::MeshOffsetTransform, offset);
	}

	//Setup bounding sphere for frustum culling
	AglBoundingSphere sphere = p_modelResource->meshHeader.boundingSphere;

	//Correct? No? Seems like this should not be done
	//sphere.position.transform(offset->offset.inverse());

	BoundingSphere* bs = new BoundingSphere(sphere);
	p_entity->addComponent(ComponentType::BoundingSphere, bs);

	BoundingBox* bb = new BoundingBox(p_modelResource->meshHeader.minimumOBB);
	p_entity->addComponent(ComponentType::BoundingBox, bb);
}

void LoadMeshSystemClient::setUpChildCollision( Entity* p_entity, 
											   ModelResource* p_modelResource, 
											   BodyInitData* p_rootRigidBodyData, 
											   PhysicsBody* p_rootPhysicsBody, 
											   AglMatrix& baseTransform )
{
	MeshOffsetTransform* offset = static_cast<MeshOffsetTransform*>(p_entity->getComponent(ComponentType::MeshOffsetTransform));
	if (!offset)
	{
		offset = new MeshOffsetTransform(p_modelResource->meshHeader.transform);
		p_entity->addComponent(ComponentType::MeshOffsetTransform, offset);
	}

	//Setup bounding sphere for frustum culling
	AglBoundingSphere sphere = p_modelResource->meshHeader.boundingSphere;

	//Correct? No? Seems like this should not be done
	//sphere.position.transform(offset->offset.inverse());

	BoundingSphere* bs = new BoundingSphere(sphere);
	p_entity->addComponent(ComponentType::BoundingSphere, bs);

	BoundingBox* bb = new BoundingBox(p_modelResource->meshHeader.minimumOBB);
	p_entity->addComponent(ComponentType::BoundingBox, bb);
}

void LoadMeshSystemClient::setUpParticles( Entity* p_entity, ModelResource* p_modelResource )
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

void LoadMeshSystemClient::setUpAnimation(Entity* p_entity, ModelResource* p_modelResource)
{
	if (p_modelResource->scene)
	{
		vector<AglSkeletonMapping*> sm = p_modelResource->scene->getSkeletonMappings();

		int index = -1;
		for (unsigned int i = 0; i < sm.size(); i++)
		{
			if (sm[i]->getHeader().meshID == p_modelResource->meshHeader.id)
				index = i;
		}

		if (index >= 0)
		{
			SkeletalAnimation* anim = static_cast<SkeletalAnimation*>(p_entity->getComponent(ComponentType::SkeletalAnimation));
			if (!anim)
			{
				anim = new SkeletalAnimation(0, p_modelResource->scene, p_modelResource->meshHeader.transform);
				p_entity->addComponent(anim);
				p_entity->applyComponentChanges();
			}
			else
			{
				anim->m_time = 0;
				anim->m_scene = p_modelResource->scene;
				anim->m_offset = p_modelResource->meshHeader.transform;
			}
		}
	}
}