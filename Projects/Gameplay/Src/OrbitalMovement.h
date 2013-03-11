#pragma once
#include <Component.h>
#include <AglVector3.h>
// =======================================================================================
// CircularMovement
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # CircularMovement
/// Detailed description...
/// Created on: 11-2-2013 
///---------------------------------------------------------------------------------------
class OrbitalMovement: public Component
{
public:
	OrbitalMovement(AglVector3 p_center, AglVector3 p_axis, AglVector3 p_vecFromCenter,
		float p_angularVelocity)
		: Component(ComponentType::CircularMovement)
	{
		centerPosition = p_center;
		axis = p_axis;
		axis.normalize();
		vectorFromCenter = p_vecFromCenter;
		angularVelocity = p_angularVelocity;
		angle = 0.0f;
	}

public:
	AglVector3 centerPosition;
	AglVector3 axis;
	AglVector3 vectorFromCenter;
	float angularVelocity;
	float angle;
};