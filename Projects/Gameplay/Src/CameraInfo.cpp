#include "CameraInfo.h"

// The include below can't be in h-file. Causes linker error.
#include <CamMatrixerUtil.h>

CameraInfo::CameraInfo( float p_aspectRatio )
{
	m_viewMatrix = AglMatrix::identityMatrix();
	m_projMatrix = AglMatrix::identityMatrix();
	m_camMatrix = AglMatrix::identityMatrix();

	m_pos = AglVector3( 0.0f, 0.0f, -3.0f );
	m_lookAt = AglVector3( 0.0f, 0.0f, 0.0f );
	m_up = AglVector3( 0.0f, 1.0f, 0.0f );

	float fovAsRadians = 3.14f/2.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;
	CHANGETHIS::SetProjMatrix( m_projMatrix, fovAsRadians, p_aspectRatio, nearClip, farClip );

	m_camMatrix = AglMatrix::transpose( AglMatrix::identityMatrix() * m_viewMatrix * m_projMatrix );
}


CameraInfo::~CameraInfo()
{
}
