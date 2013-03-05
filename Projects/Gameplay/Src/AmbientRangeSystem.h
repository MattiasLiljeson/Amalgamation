#pragma once
#include <EntitySystem.h>

// =======================================================================================
//                                      AmbientRangeSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # AmbientRangeSystem
/// Detailed description.....
/// Created on: 4-3-2013 
///---------------------------------------------------------------------------------------

class AmbientRangeSystem : public EntitySystem
{
public:
	AmbientRangeSystem();
	virtual ~AmbientRangeSystem();

	void processEntities( const vector<Entity*>& p_entities );
};