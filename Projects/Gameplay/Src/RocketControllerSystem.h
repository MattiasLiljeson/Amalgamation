#pragma once

#include <EntitySystem.h>
#include "StandardRocket.h"
#include "Transform.h"

// =======================================================================================
//                                      RocketControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief
///        
/// # RocketControllerSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
class RocketControllerSystem: public EntitySystem
{
public:
	RocketControllerSystem();
	~RocketControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );
};