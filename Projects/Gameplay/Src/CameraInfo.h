#pragma once

#include "Component.h"
#include <AglMatrix.h>

class CameraInfo : public Component
{
public:
	CameraInfo( float p_aspectRatio, float p_fieldOfViewAsRadians=0.785398163f, 
		float p_nearPlane=0.1f, float p_farPlane=10000.0f);
	~CameraInfo(){};

	void createPerspectiveMatrix();
	void createPerspectiveMatrix( float p_aspectRatio);
	void createPerspectiveMatrix( float p_aspectRatio , 
		float p_fieldOfViewAsRadians);
	void createPerspectiveMatrix( float p_aspectRatio , 
		float p_fieldOfViewAsRadians, 
		float p_nearClip, 
		float p_farClip);

	AglMatrix m_projMat;
	AglVector3 m_ambientColor;
	AglVector3 m_fogColor;
	float	m_fogNearPlaneClosenessPercentage;
	float	m_fogFarPlaneClosenessPercentage;
	float	m_farPlane;
	float	m_nearPlane;
	float	m_aspectRatio;
	float	m_fieldOfViewAsRadians;
	int		m_shadowMapIdx;
};