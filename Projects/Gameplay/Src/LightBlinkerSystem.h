#pragma once
#include <EntitySystem.h>

// =======================================================================================
//	LightBlinkerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # LightBlinkerSystem
/// Detailed description...
/// Created on: 28-1-2013 
///---------------------------------------------------------------------------------------

class LightBlinkerSystem: public EntitySystem
{
public:
	LightBlinkerSystem();
	~LightBlinkerSystem();

	void processEntities( const vector<Entity*>& p_entities );
	void inserted( Entity* p_entity );

};