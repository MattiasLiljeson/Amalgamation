#pragma once

#include <EntitySystem.h>

class InterpolationComponent;
class Transform;
// =======================================================================================
//	InterpolationSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief
///
/// # InterpolationSystem
/// Created on: 1-2-2013 
///---------------------------------------------------------------------------------------

class InterpolationSystem: public EntitySystem
{
public:
	InterpolationSystem();

	~InterpolationSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void initialize();
private:
	void handleTransformInterpolation( InterpolationComponent* p_interpolation, 
		Transform* p_transform );
};