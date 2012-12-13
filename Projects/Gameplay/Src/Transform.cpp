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

	calcCompMatrix();
}

Transform::~Transform()
{

}

AglVector3 Transform::getTranslation() const
{
	return m_translation;
}

void Transform::setTranslation( const AglVector3 p_translation )
{
	m_translation = p_translation;
	calcCompMatrix();
}

AglVector3 Transform::getScale() const
{
	return m_scale;
}

void Transform::setScale( const AglVector3 p_scale )
{
	m_scale = p_scale;
	calcCompMatrix();
}

AglQuaternion Transform::getRotation() const
{
	return m_rotation;
}

void Transform::setRotation( const AglQuaternion p_rotation )
{
	m_rotation = p_rotation;
	calcCompMatrix();
}

AglMatrix Transform::getMatrix() const
{
	return m_compositionMatrix;
}

InstanceVertex Transform::getInstanceVertex() const
{
	return m_instanceVertex;
}

InstanceVertex& Transform::getInstanceVertexRef()
{
	return m_instanceVertex;
}

InstanceVertex* Transform::getInstanceVertexPtr()
{
	return &m_instanceVertex;
}

void Transform::calcCompMatrix()
{
	AglMatrix::componentsToMatrix(
		m_compositionMatrix,
		m_scale,
		m_rotation,
		m_translation);

	AglMatrix transMat = AglMatrix::transpose( m_compositionMatrix );
	for( int i=0; i<16; i++ )
	{
		m_instanceVertex.worldTransform[i] = transMat[i];
	}
}