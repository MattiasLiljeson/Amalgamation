#pragma once
#include <Component.h>
#include <AglVector3.h>
#include <AglQuaternion.h>
#include <ComponentFactory.h>
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
	AxisRotate();
	AxisRotate(AglVector3 p_axis, AglVector3 p_startRotation,
		AglQuaternion p_originRotation, float p_angularVelocity, float p_angle=0.0f);
	void init( vector<ComponentData> p_initData ) final;

public:
	AglVector3 axis;
	AglVector3 startVector;
	AglQuaternion originRotation;
	float angularVelocity;
	float angle;

public:
	static ComponentRegister<AxisRotate> s_reg;
};