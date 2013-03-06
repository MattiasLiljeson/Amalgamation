#pragma once
#include <Component.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
// =======================================================================================
// AxisRotate
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Rotate around a given axis.
/// 
/// Created on: 19-2-2013 
///---------------------------------------------------------------------------------------
class AxisRotate: public Component
{
public:
	AxisRotate(AglVector3 p_axis, AglVector3 p_startRotation,
		AglQuaternion p_originRotation, float p_angularVelocity, float p_angle=0.0f)
		: Component(ComponentType::AxisRotate)
	{
		axis = p_axis;
		axis.normalize();
		startVector = p_startRotation;
		originRotation = p_originRotation;
		angularVelocity = p_angularVelocity;
		angle = p_angle;
	}

public:
	AglVector3 axis;
	AglVector3 startVector;
	AglQuaternion originRotation;
	float angularVelocity;
	float angle;
};