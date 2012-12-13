#pragma once

#include "Component.h"
#include <AglMatrix.h>
#include <GraphicsWrapper.h>
#include <RendererSceneInfo.h>

class CameraInfo : public Component
{
public:
	CameraInfo( float p_aspectRatio );
	~CameraInfo();

	//AglMatrix m_viewMatrix;
	AglMatrix m_projMat;
	//AglMatrix m_camMatrix; 

	//AglVector3 m_pos;
	AglVector3 m_lookAt;
	AglVector3 m_up;
};

