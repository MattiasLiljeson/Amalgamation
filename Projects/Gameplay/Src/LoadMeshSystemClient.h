#pragma once

#include "LoadMeshSystem.h"

class GraphicsBackendSystem;
class Transform;
struct ModelResource;

// =======================================================================================
//                                      LoadMeshSystemClient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief System that processes entities with LoadMeshJobComponents and loads the
/// specified mesh from file, which can result in additional components on the entity as
/// well as new entities.
/// Version for client, creates modelresources with buffers.
///        
/// # LoadMeshSystemClient
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class LoadMeshSystemClient : public LoadMeshSystem
{
public:
	LoadMeshSystemClient(GraphicsBackendSystem* p_gfxBackend);
	virtual ~LoadMeshSystemClient();
	virtual void initialize();
private:
	///-----------------------------------------------------------------------------------
	/// A way to generate model resources
	/// \return vector<ModelResource*>*
	///-----------------------------------------------------------------------------------
	virtual vector<ModelResource*>* createModels(const string& p_filename, 
		const string& p_filePath,
		bool p_isPrimitive);

	virtual void setUpRootCollision(Entity* p_entity, ModelResource* p_modelResource);

	virtual void setUpChildCollision(Entity* p_entity, ModelResource* p_modelResource, 
		BodyInitData* p_rootRigidBodyData,
		PhysicsBody* p_rootPhysicsBody,
		AglMatrix& baseTransform);

	virtual void setUpParticles(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpAnimation(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpLights(Entity* p_entity, ModelResource* p_modelResource);

	GraphicsBackendSystem* m_gfxBackend;
};