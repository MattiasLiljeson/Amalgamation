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
class CircularMovement: public Component
{
public:
	CircularMovement(AglVector3 p_center, float p_radius, float p_initialAngle,
		float p_angularVelocity)
		: Component(ComponentType::CircularMovement)
	{
		centerPosition = p_center;
		radius = p_radius;
		angle = p_initialAngle;
		angularVelocity = p_angularVelocity;
	}

public:
	AglVector3 centerPosition;
	float radius;
	float angle;
	float angularVelocity;
};