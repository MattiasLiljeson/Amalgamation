#include "CameraInfo.h"

CameraInfo::CameraInfo( float p_aspectRatio )
{
	m_projMat = AglMatrix::identityMatrix();

	float fovAsRadians = 3.14f/2.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;
	m_projMat = AglMatrix::createPerspectiveMatrix(fovAsRadians,p_aspectRatio,nearClip,farClip);
}


CameraInfo::~CameraInfo()
{
}
