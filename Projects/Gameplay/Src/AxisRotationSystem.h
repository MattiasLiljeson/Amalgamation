#pragma once
#include <EntitySystem.h>
// =======================================================================================
// AxisRotationSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # AxisRotationSystem
/// Detailed description...
/// Created on: 19-2-2013 
///---------------------------------------------------------------------------------------
class AxisRotationSystem: public EntitySystem
{
public:
	AxisRotationSystem();
	virtual void processEntities(const vector<Entity*>& p_entities) final;

};