#include "LoadMeshSystemServer.h"

#include <ModelBaseFactory.h>
#include "PhysicsBody.h"
#include "BodyInitData.h"

LoadMeshSystemServer::LoadMeshSystemServer() : 
	LoadMeshSystem()
{
	m_modelBaseFactory = new ModelBaseFactory();
}

LoadMeshSystemServer::~LoadMeshSystemServer()
{
	delete m_modelBaseFactory;
}

void LoadMeshSystemServer::initialize()
{

}

vector<ModelResource*>* LoadMeshSystemServer::createModels( const string& p_filename, 
														   const string& p_filePath, 
														   bool p_isPrimitive )
{
	return m_modelBaseFactory->createModelResources(p_filename,&p_filePath);
}


void LoadMeshSystemServer::setUpChildCollision( Entity* p_entity, 
										 ModelResource* p_modelResource, 
										 BodyInitData* p_rootRigidBodyData,
										 PhysicsBody* p_rootPhysicsBody,
										 AglMatrix& baseTransform )
{
	if (p_rootRigidBodyData)
	{
		PhysicsBody* pb = new PhysicsBody();
		if (p_rootRigidBodyData->m_compound)
			pb->setParentId(p_rootPhysicsBody->m_id);
		p_entity->addComponent( ComponentType::PhysicsBody, 
			pb );


		BodyInitData* b = new BodyInitData(AglVector3(30, 0, 0),
			AglQuaternion::identity(),
			AglVector3(1, 1, 1), AglVector3(0, 0, 0), 
			AglVector3(0, 0, 0), 0);
		// Copy from parent
		b->m_position		= baseTransform.GetTranslation();
		b->m_orientation	= baseTransform.GetRotation();
		b->m_scale			= baseTransform.GetScale();
		b->m_velocity			= p_rootRigidBodyData->m_velocity;
		b->m_angularVelocity	= p_rootRigidBodyData->m_angularVelocity;
		b->m_collisionEnabled	= p_rootRigidBodyData->m_collisionEnabled;
		b->m_compound			= p_rootRigidBodyData->m_compound;
		b->m_impulseEnabled		= p_rootRigidBodyData->m_impulseEnabled;

		b->m_type = p_rootRigidBodyData->m_type;

		p_entity->addComponent( ComponentType::BodyInitData, b);
	}
}