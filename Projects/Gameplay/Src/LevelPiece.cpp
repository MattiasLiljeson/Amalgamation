#include "LevelPiece.h"

#include <ConnectionPointCollection.h>
#include "Transform.h"
#include <cstdlib>
#include <AglMesh.h>
#include <ModelResource.h>

LevelPiece::LevelPiece(int p_typeId, ModelResource* p_modelResource, Transform* p_transform )
{
	m_modelResource = p_modelResource;
	m_transform		= p_transform;
	m_typeId		= p_typeId;

	int maxChildCount = p_modelResource->connectionPoints.m_collection.size();
	m_childSlotsOccupied.resize(maxChildCount);
	m_children.resize(maxChildCount, NULL);
	m_connectionPoints.resize(maxChildCount);

	updateConnectionPoints();
	updateBoundingVolumes();
}


LevelPiece::~LevelPiece()
{
	// Delete main piece now, as it will be used as a copy for the created entity later,
	// and not be owned by ES
	deleteMainTransform();
}

vector<int> LevelPiece::findFreeConnectionPointSlots()
{
	vector<int> freeSlots;
	// Check for unoccupied slots
	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		if ( !m_childSlotsOccupied[i] )
			freeSlots.push_back(i);
	}
	return freeSlots;
}

Transform* LevelPiece::getTransform() const
{
	return m_transform;
}

int LevelPiece::getTypeId() const
{
	return m_typeId;
}

const Transform* LevelPiece::getChild( int p_inSlot ) const
{
	return m_children[p_inSlot];
}

void LevelPiece::setChild( int p_inSlot, Transform* p_transform )
{
	m_children[p_inSlot]			= p_transform;
	m_childSlotsOccupied[p_inSlot]	= true;
}

bool LevelPiece::connectTo( LevelPiece* p_targetPiece, int p_targetSlot )
{
	// TODO: ? Checking collision before pieces are created to reduce overhead?

	// To prevent issues with scale, fetch this scale temporarily
	AglVector3 tempScale = m_transform->getScale();

	// Rotate the connection point arbitrarily around its forward!
	//Transform transform = m_connectionPoints[0];
	//transform.setRotation(transform.getRotation() 
	//	* AglQuaternion::constructFromAxisAndAngle(AglVector3::forward(), (rand() % 360) * 3.1415f / 180.0f));
	//m_connectionPoints[0] = transform;

	// 1) Transform this piece and all its connection points with the inverse matrix of the
	// used this-connector.
	m_transform->setMatrix(m_transform->getMatrix() * m_connectionPoints[0].getMatrix().inverse());
	m_transform->setScale(tempScale);
	updateConnectionPoints();

	// 1.5) if step2 fails, flip forward vector using target connector and create matrix blä.
	//AglMatrix mat = p_targetPiece->getConnectionPointMatrix(p_targetSlot);
	//mat.SetForward( mat.GetBackward() );
	Transform temp = p_targetPiece->getConnectionPoint(p_targetSlot);
	temp.setForwardDirection( -temp.getForward() );
	//temp.setForwardDirection( AglVector3::up() );

	// Test: Extract translation and rotation from the target piece connector, and then
	// rotate it PI radians around its axis
	//AglMatrix mat( AglVector3::one(), 
	//				temp.getRotation() * AglQuaternion::constructFromAxisAndAngle(temp.getForward(), 3.1415f),
	//				temp.getTranslation() );

	// 2) Transform this piece and connection points with target piece connector matrix or blä.
	m_transform->setMatrix( m_transform->getMatrix() * temp.getMatrix() );
	//m_transform->setMatrix( m_transform->getMatrix() * mat );
	m_transform->setScale(temp.getScale());
	updateConnectionPoints();

	// Set this connection to be occupied!
	p_targetPiece->setChild(p_targetSlot, m_transform);
	setChild(0, p_targetPiece->getTransform());

	updateBoundingVolumes();

	return true;
}

AglMatrix LevelPiece::getLocalConnectionPointMatrix( int p_vectorIndex, E_Space p_inSpace/*=Space_LOCAL*/ )
{
	if (p_inSpace == Space_LOCAL)
		return m_modelResource->connectionPoints.m_collection[p_vectorIndex];
	else
		return m_modelResource->connectionPoints.m_collection[p_vectorIndex] *
				m_transform->getMatrix();
}

void LevelPiece::updateConnectionPoints()
{
	for (unsigned int i = 0; i < m_connectionPoints.size(); i++)
	{
		m_connectionPoints[i] = Transform(
			m_modelResource->connectionPoints.m_collection[i] *
			m_transform->getMatrix());
	}
}

AglMatrix LevelPiece::getConnectionPointMatrix( int p_vectorIndex )
{
	return m_connectionPoints[p_vectorIndex].getMatrix();
}

Transform LevelPiece::getConnectionPoint( int p_vectorIndex )
{
	return m_connectionPoints[p_vectorIndex];
}

const AglOBB& LevelPiece::getBoundingBox() const
{
	return m_boundingBox;
}

void LevelPiece::updateBoundingVolumes()
{
	// Update the bounding sphere!
	m_boundingSphere.position	= m_transform->getTranslation() + 
									m_modelResource->meshHeader.boundingSphere.position;
	m_boundingSphere.radius		= m_transform->getScale().x * 
									m_modelResource->meshHeader.boundingSphere.radius;

	// Updating bounding box.
	m_boundingBox.size = m_transform->getScale() * 
							m_modelResource->meshHeader.minimumOBB.size;

	AglMatrix mat(AglVector3::one(), m_transform->getRotation(), m_transform->getTranslation());
	m_boundingBox.world = m_modelResource->meshHeader.minimumOBB.world * mat;
}

const AglBoundingSphere& LevelPiece::getBoundingSphere() const
{
	return m_boundingSphere;
}
const ModelResource* LevelPiece::getModelResource() const
{
	return m_modelResource;
}

void LevelPiece::deleteMainTransform()
{
	delete m_transform;
	m_transform = NULL;
}
