#pragma once

#include "Component.h"
#include <AglMatrix.h>

class CameraInfo : public Component
{
public:
	CameraInfo( float p_aspectRatio, float p_fieldOfViewAsRadians=0.785398163f, 
		float p_nearPlane=0.01f, float p_farPlane=10000.0f);
	~CameraInfo(){};

	void createPerspectiveMatrix();
	void createPerspectiveMatrix( float p_aspectRatio , 
		float p_fieldOfViewAsRadians=0.785398163f, 
		float p_nearClip=0.01f, 
		float p_farClip=10000.0f);

	AglMatrix m_projMat;
	float m_farPlane;
	float m_nearPlane;
	float m_aspectRatio;
	float m_fieldOfViewAsRadians;
};

