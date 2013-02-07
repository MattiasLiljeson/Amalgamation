#pragma once
#include <EntitySystem.h>
#include <vector>

struct AglMatrix;

// =======================================================================================
//                                      ShadowSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ShadowSystem
/// Detailed description.....
/// Created on: 31-1-2013 
///---------------------------------------------------------------------------------------

class ShadowSystem : public EntitySystem
{
public:
	ShadowSystem();
	~ShadowSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void render();

	unsigned int getNumberOfShadowCameras() const;
	Entity*	getShadowEntity(const unsigned int p_index);
	AglMatrix getViewProjection(const unsigned int p_index);
	int getShadowIdx(const unsigned int p_index);
private:
	vector<Entity*> m_shadowCameras;
};