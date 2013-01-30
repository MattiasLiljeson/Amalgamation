#pragma once

#include "Component.h"
#include <AglMatrix.h>

class CameraInfo : public Component
{
public:
	CameraInfo( float p_aspectRatio, float p_nearPlane=0.1f, float p_farPlane=1000.0f);
	~CameraInfo(){};

	void createPerspectiveMatrix();
	void createPerspectiveMatrix( float p_aspectRatio , float p_nearClip = 0.1f, 
		float p_farClip = 10000.0f);

	AglMatrix m_projMat;
	float m_farPlane;
	float m_nearPlane;
	float m_aspectRatio;
private:
	float m_fovAsRadians;
};

