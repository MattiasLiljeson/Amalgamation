#pragma once
#include <EntitySystem.h>
#include <map>
using namespace std;
// =======================================================================================
// ShieldPlaterSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ShieldPlaterSystem
/// Detailed description...
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------
class ShieldPlaterSystem: public EntitySystem
{
public:
	ShieldPlaterSystem();
	void processEntities( const vector<Entity*>& p_entities ) final;
	void inserted( Entity* p_entity ) final;
	// NOTE: (Johan) Don't forget deleted(Entity*) event.

private:
	void circularRandom(float* p_spawnX, float* p_spawnY, bool p_warpCompensation=false);

private:
	map<Entity*, vector<Entity*> > m_shieldPlates;
};