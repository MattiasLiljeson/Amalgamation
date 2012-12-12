#include "Transform.h"

Transform::Transform()
{
	m_translation.x = 0.0f;
	m_translation.y = 0.0f;
	m_translation.z = 0.0f;

	m_scale.x = 1.0f;
	m_scale.y = 1.0f;
	m_scale.z = 1.0f;

	m_rotation = AglQuaternion::identity();

	//m_orientation.x = 0.0f;
	//m_orientation.y = 0.0f;
	//m_orientation.z = 0.0f;

	calcCompMatrix();
}

Transform::Transform( float p_posX, float p_posY, float p_posZ )
{
	m_translation.x = p_posX;
	m_translation.y = p_posY;
	m_translation.z = p_posZ;

	m_scale.x = 1.0f;
	m_scale.y = 1.0f;
	m_scale.z = 1.0f;

	m_rotation = AglQuaternion::identity();

	//m_orientation.x = 0.0f;
	//m_orientation.y = 0.0f;
	//m_orientation.z = 0.0f;

	calcCompMatrix();
}

Transform::~Transform()
{

}

AglVector3* Transform::getTranslation()
{
	return &m_translation;
}

AglVector3* Transform::getScale()
{
	return &m_scale;
}

AglQuaternion* Transform::getRotation()
{
	return &m_rotation;
}

AglMatrix* Transform::getMatrix()
{
	return &m_compositionMatrix;
}

AglMatrix* Transform::getTransposedMatrix()
{
	return &m_transposedCompositionMatrix;
}

void Transform::calcCompMatrix()
{
	AglMatrix::componentsToMatrix(
		m_compositionMatrix,
		m_scale,
		m_rotation,
		m_translation);
	m_transposedCompositionMatrix = AglMatrix::transpose( m_compositionMatrix );
}
