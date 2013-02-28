#pragma once
#include <EntitySystem.h>
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
	void inserted( Entity* p_entity ) final;

private:
	void circularRandom(float* p_spawnX, float* p_spawnY, bool p_warpCompensation=false );
};