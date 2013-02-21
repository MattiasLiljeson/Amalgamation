#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                      EnvironmentSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System that collects info of all environmentvalues on the map and combines
/// them to a value for the camera. Environment values are values for the renderer usually
/// considered global, but that can be changed on a per-chamber basis in this game. 
/// For example fog and ambient lighting.
///        
/// # EnvironmentSystem
/// Detailed description.....
/// Created on: 20-2-2013 
///---------------------------------------------------------------------------------------

class EnvironmentSystem : public EntitySystem
{
public:
	EnvironmentSystem();
	virtual ~EnvironmentSystem();

	virtual void initialize();

	virtual void processEntities( const vector<Entity*>& p_entities );
protected:
private:
};