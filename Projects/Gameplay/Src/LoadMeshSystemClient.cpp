#include "LoadMeshSystemClient.h"

#include "GraphicsBackendSystem.h"
#include "LoadMesh.h"
#include <ModelResource.h>
#include <GraphicsWrapper.h>
#include "Transform.h"
#include "RenderInfo.h"
#include "EntityParent.h"
#include "ConnectionPointSet.h"
#include "BodyInitData.h"
#include "PhysicsBody.h"
#include <AglSkeletonMapping.h>
#include "SkeletalAnimation.h"

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

void LoadMeshSystemClient::setUpChildCollision( Entity* p_entity, 
											   ModelResource* p_modelResource, 
											   BodyInitData* p_rootRigidBodyData, 
											   PhysicsBody* p_rootPhysicsBody, 
											   AglMatrix& baseTransform )
{
	// Do nothing on client here, trolololo
	// Check server version for how to set up collision data
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
			SkeletalAnimation* anim = new SkeletalAnimation(0, p_modelResource->scene);
			p_entity->addComponent(anim);
		}
	}
}
