#pragma once

#include "Component.h"
#include <AglMatrix.h>
struct AglMatrix;

class CameraInfo : public Component
{
public:
	CameraInfo( float p_aspectRatio );
	~CameraInfo(){};

	void createPerspectiveMatrix( float p_aspectRatio , float p_nearClip = 0.1f, 
		float p_farClip = 10000.0f);

	AglMatrix m_projMat;
	float m_farPlane;
	float m_nearPlane;
};

