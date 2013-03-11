#pragma once

#include "LoadMeshSystem.h"

class ModelBaseFactory;
class Transform;
struct ModelResource;

// =======================================================================================
//                                      LoadMeshSystemServer
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief System that processes entities with LoadMeshJobComponents and loads the
/// specified mesh from file, which can result in additional components on the entity as
/// well as new entities.
/// Version for server, creates modelresources with collision.
///        
/// # LoadMeshSystemClient
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class LoadMeshSystemServer : public LoadMeshSystem
{
public:
	LoadMeshSystemServer();
	virtual ~LoadMeshSystemServer();
	virtual void initialize();
	///-----------------------------------------------------------------------------------
	/// A way to generate model resources
	/// \return vector<ModelResource*>*
	///-----------------------------------------------------------------------------------
	virtual vector<ModelResource*>* createModels(const string& p_filename, 
		const string& p_filePath,
		bool p_isPrimitive);

private:
	virtual void setUpChildCollision(Entity* p_entity, ModelResource* p_modelResource, 
		BodyInitData* p_rootRigidBodyData,
		PhysicsBody* p_rootPhysicsBody,
		AglMatrix& baseTransform);

	virtual void setUpParticles(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpAnimation(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpLights(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpAmbientRangeSound(Entity* p_entity, ModelResource* p_modelResource);

	ModelBaseFactory* m_modelBaseFactory;
};