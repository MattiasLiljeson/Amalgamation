#pragma once

#include <EntitySystem.h>

class Transform;
struct ModelResource;
class BodyInitData;
class PhysicsBody;
struct AglMatrix;


// =======================================================================================
//                                      LoadMeshSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief System that processes entities with LoadMeshJobComponents and loads the
/// specified mesh from file, which can result in additional components on the entity as
/// well as new entities.
///
/// Base class with pure virtual meshresource creation method.
///
/// # LoadMeshSystem
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class LoadMeshSystem : public EntitySystem
{
public:
	LoadMeshSystem();
	virtual ~LoadMeshSystem();
	virtual void processEntities( const vector<Entity*>& p_entities );
	virtual void initialize();
	//virtual void inserted( Entity* p_entity );
private:

	///-----------------------------------------------------------------------------------
	/// Set data for root entity, ie. the one processed
	/// \param p_entity
	/// \return int
	///-----------------------------------------------------------------------------------
	void setRootData(Entity* p_entity, ModelResource* p_modelResource, 
		Transform* p_outTransform);

	///-----------------------------------------------------------------------------------
	/// Create children entities, if any are specified
	/// \param p_modelResources
	/// \param p_numEntities
	/// \return void
	///-----------------------------------------------------------------------------------
	void createChildrenEntities(vector<ModelResource*>* p_modelResources,
		Entity* p_rootEntity);

	///-----------------------------------------------------------------------------------
	/// A way to generate model resources
	/// \return vector<ModelResource*>*
	///-----------------------------------------------------------------------------------
	virtual vector<ModelResource*>* createModels(const string& p_filename, 
		const string& p_filePath,
		bool p_isPrimitive)=0;

	virtual void setUpRenderInfo(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpConnectionPoints(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpSpawnPoints(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpLights(Entity* p_entity, ModelResource* p_modelResource)=0;
	virtual void setUpParticles(Entity* p_entity, ModelResource* p_modelResource) = 0;
	virtual void setUpAmbientRangeSound(Entity* p_entity, ModelResource* p_modelResource) = 0;

	virtual void setUpRootCollision(Entity* p_entity, ModelResource* p_modelResource);
	virtual void setUpChildCollision(Entity* p_entity, ModelResource* p_modelResource, 
									 BodyInitData* p_rootRigidBodyData,
									 PhysicsBody* p_rootPhysicsBody,
									 AglMatrix& baseTransform)=0;
	virtual void setUpAnimation(Entity* p_entity, ModelResource* p_modelResource) = 0;
};