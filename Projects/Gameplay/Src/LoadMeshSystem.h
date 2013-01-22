#pragma once

#include <EntitySystem.h>

class GraphicsBackendSystem;

// =======================================================================================
//                                      LoadMeshSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief System that processes entities with LoadMeshJobComponents and loads the
/// specified mesh from file, which can result in additional components on the entity as
/// well as new entities.
///        
/// # LoadMeshSystem
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class LoadMeshSystem : public EntitySystem
{
public:
	LoadMeshSystem(GraphicsBackendSystem* p_gfxBackend);
	virtual ~LoadMeshSystem();
	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
private:

	///-----------------------------------------------------------------------------------
	/// Setr data for root entity, ie. the one processed
	/// \param p_entity
	/// \return int
	///-----------------------------------------------------------------------------------
	int setRootData(Entity* p_entity, ModelResource* p_modelResource, 
					Transform* p_outTransform);

	///-----------------------------------------------------------------------------------
	/// Create children entities, if any are specified
	/// \param p_modelResources
	/// \param p_numEntities
	/// \return void
	///-----------------------------------------------------------------------------------
	void createChildrenEntities(vector<ModelResource*>* p_modelResources, 
								   int p_rootId, Transform* p_rootTransform);

	GraphicsBackendSystem* m_gfxBackend;
};