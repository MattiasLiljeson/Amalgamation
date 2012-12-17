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

	AglMatrix m_projMat;
};

