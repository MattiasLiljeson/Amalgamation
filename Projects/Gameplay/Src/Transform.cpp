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

Transform::Transform(const AglVector3& p_translation, const AglQuaternion& p_rotation, 
					 const AglVector3& p_scale)
{
	m_translation = p_translation;
	m_rotation = p_rotation;
	m_scale = p_scale;

	calcCompMatrix();
}

Transform::Transform(const AglMatrix& p_matrix)
{
	setMatrix(p_matrix);
}

Transform::~Transform()
{

}

const AglVector3& Transform::getTranslation() const
{
	return m_translation;
}

void Transform::setTranslation( const AglVector3& p_translation )
{
	m_translation = p_translation;
	calcCompMatrix();
}

const AglVector3& Transform::getScale() const
{
	return m_scale;
}

void Transform::setScale( const AglVector3& p_scale )
{
	m_scale = p_scale;
	calcCompMatrix();
}

const AglQuaternion& Transform::getRotation() const
{
	return m_rotation;
}

void Transform::setRotation( const AglQuaternion& p_rotation )
{
	m_rotation = p_rotation;
	calcCompMatrix();
}

void Transform::setForward( const AglVector3& p_forward )
{
	m_compositionMatrix.SetForward(p_forward);
}

void Transform::setRight( const AglVector3& p_right )
{
	m_compositionMatrix.SetRight(p_right);
}

void Transform::setUp( const AglVector3& p_up )
{
	m_compositionMatrix.SetUp(p_up);
}

void Transform::setMatrix(const AglMatrix& p_matrix)
{
	m_compositionMatrix = p_matrix;
	calcComponents();
}

const AglMatrix& Transform::getMatrix() const
{
	return m_compositionMatrix;
}

InstanceData Transform::getInstanceVertex() const
{
	return m_instanceData;
}

InstanceData& Transform::getInstanceDataRef()
{
	return m_instanceData;
}

InstanceData* Transform::getInstanceDataPtr()
{
	return &m_instanceData;
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
		m_instanceData.worldTransform[i] = transMat[i];
	}
}

void Transform::calcComponents()
{
	// implement this for scale and rotation as well
	m_translation = m_compositionMatrix.GetTranslation();

	AglMatrix transMat = AglMatrix::transpose( m_compositionMatrix );
	for( int i=0; i<16; i++ )
	{
		m_instanceData.worldTransform[i] = transMat[i];
	}
}


