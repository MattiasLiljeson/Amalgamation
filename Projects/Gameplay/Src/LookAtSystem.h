#pragma once

#include <EntitySystem.h>


// =======================================================================================
//                                   LookAtSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Changes transforms to look at a target, further behaviour can be enabled
/// using tags for now.
///        
/// # LookAtSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class LookAtSystem : public EntitySystem
{
public:
	LookAtSystem();
	~LookAtSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	void adaptDistanceBasedOnModules(Entity* p_entity);

};
