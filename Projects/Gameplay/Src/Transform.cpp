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

void Transform::setForwardDirection( const AglVector3& p_forward )
{
	AglVector3 t_up = m_compositionMatrix.GetUp();
	t_up = t_up - p_forward * AglVector3::dotProduct(t_up,p_forward) / AglVector3::dotProduct(t_up,t_up);
	t_up.normalize();
	AglVector3 t_right = AglVector3::crossProduct(t_up,p_forward);
    t_right.normalize();

	m_compositionMatrix.SetForward(p_forward);
 	m_compositionMatrix.SetRight(t_right);
 	m_compositionMatrix.SetUp(t_up);

	calcComponents(false,true,true);

	setScale(m_scale);
}

// void Transform::setRightDirection( const AglVector3& p_right )
// {
// 	// Not implemented yet
// }

// void Transform::setUpDirection( const AglVector3& p_up )
// {
// 	// Not implemented yet
// }

void Transform::setMatrix(const AglMatrix& p_matrix)
{
	m_compositionMatrix = p_matrix;
	calcComponents();
}


AglVector3 Transform::getForward() const
{
	return m_compositionMatrix.GetForward();
}

AglVector3 Transform::getRight() const
{
	return m_compositionMatrix.GetRight();
}

AglVector3 Transform::getUp() const
{
	return m_compositionMatrix.GetUp();
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

void Transform::calcComponents(bool p_calcScale/*=true*/, bool p_calcRotation/*=true*/, bool p_calcTranslation/*=true*/)
{
	AglVector3* temp_scale = &m_scale;
	AglVector3* temp_translation = &m_translation;
	AglQuaternion* temp_rotation = &m_rotation;
	if (!p_calcScale) temp_scale = &AglVector3::one();
	if (!p_calcTranslation) temp_translation = &AglVector3::zero();
	if (!p_calcRotation) temp_rotation = &AglQuaternion::identity();
	m_compositionMatrix.toComponents(*temp_scale,*temp_rotation,*temp_translation);

	AglMatrix transMat = AglMatrix::transpose( m_compositionMatrix );
	for( int i=0; i<16; i++ )
	{
		m_instanceData.worldTransform[i] = transMat[i];
	}
}
