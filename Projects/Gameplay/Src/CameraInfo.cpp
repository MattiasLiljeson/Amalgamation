#include "CameraInfo.h"

CameraInfo::CameraInfo(float p_aspectRatio, 
					   float p_fieldOfViewAsRadians/* =0.785398163f */, 
					   float p_nearPlane/* =0.1f */, 
					   float p_farPlane/* =10000.0f */)
	: Component( ComponentType::CameraInfo )
{
	m_projMat		= AglMatrix::identityMatrix();
	m_aspectRatio	= p_aspectRatio;
	m_fieldOfViewAsRadians	= p_fieldOfViewAsRadians;
	m_nearPlane		= p_nearPlane;
	m_farPlane		= p_farPlane;
	m_shadowMapIdx	= -1;
	m_ambientColor	= AglVector3(0.259f,0.169f,0.157f);
	m_fogColor		= AglVector3(0.38f,0.58f,0.3764f);
	m_fogNearPlaneClosenessPercentage	= 1.0f;
	m_fogFarPlaneClosenessPercentage	= 1.0f;
	createPerspectiveMatrix();
}

void CameraInfo::createPerspectiveMatrix(){
	m_projMat = AglMatrix::createPerspectiveMatrix(m_aspectRatio,m_fieldOfViewAsRadians,
		m_nearPlane,
		m_farPlane);
}

void CameraInfo::createPerspectiveMatrix(float p_aspectRatio , 
										 float p_fieldOfViewAsRadians/* =0.785398163f */, 
										 float p_nearClip/* =0.1f */, 
										 float p_farClip/* =10000.0f */)
{
	m_aspectRatio = p_aspectRatio;
	m_fieldOfViewAsRadians = p_fieldOfViewAsRadians;
	m_nearPlane = p_nearClip;
	m_farPlane	= p_farClip;
	createPerspectiveMatrix();
}
