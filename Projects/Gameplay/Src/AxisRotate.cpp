#include "AxisRotate.h"

ComponentRegister<AxisRotate> AxisRotate::s_reg("AxisRotate");

AxisRotate::AxisRotate()
	: Component(ComponentType::AxisRotate)
{
}

AxisRotate::AxisRotate( AglVector3 p_axis, AglVector3 p_startRotation,
	AglQuaternion p_originRotation, float p_angularVelocity, float p_angle/*=0.0f*/ )
	: Component(ComponentType::AxisRotate)
{
	axis = p_axis;
	axis.normalize();
	startVector = p_startRotation;
	originRotation = p_originRotation;
	angularVelocity = p_angularVelocity;
	angle = p_angle;
}

void AxisRotate::init( vector<ComponentData> p_initData )
{
	for(unsigned int i=0; i<p_initData.size(); i++)
	{
		if(p_initData[i].dataName == "axis") {
			p_initData[i].getDataAsVec3(&axis.x, &axis.y, &axis.z);
		}
		else if(p_initData[i].dataName == "startVector") {
			p_initData[i].getDataAsVec3(&startVector.x, &startVector.y, &startVector.z);
		}
		else if(p_initData[i].dataName == "originRotation") {
			p_initData[i].getDataAsVec4(&originRotation.u.x,
				&originRotation.u.y,
				&originRotation.u.z,
				&originRotation.v);
		}
		else if(p_initData[i].dataName == "angularVelocity") {
			p_initData[i] >> angularVelocity;
		}
		else if(p_initData[i].dataName == "angle") {
			p_initData[i] >> angle;
		}
	}
}