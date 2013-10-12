#include "LevelPiece.h"

#include <ConnectionPointCollection.h>
#include "Transform.h"
#include <cstdlib>
#include <AglMesh.h>
#include <ModelResource.h>

LevelPiece::LevelPiece(int p_typeId, ModelResource* p_modelResource, Transform* p_transform, int p_generation )
{
	m_modelResource = p_modelResource;
	m_transform		= p_transform;
	m_typeId		= p_typeId;
	m_generation	= p_generation;

	int maxChildCount = p_modelResource->connectionPoints.m_collection.size();
	m_connectedSlotsOccupied.resize(maxChildCount);
	m_connectedPieces.resize(maxChildCount, NULL);
	m_connectionPoints.resize(maxChildCount);
	m_gates.resize(maxChildCount, -1);
	m_parent = nullptr;

	updateConnectionPoints();
	updateBoundingVolumes();
}


LevelPiece::~LevelPiece()
{
	// Delete main piece now, as it will be used as a copy for the created entity later,
	// and not be owned by ES
	m_parent = nullptr;
	m_children.clear();
	deleteMainTransform();
}

vector<int> LevelPiece::findFreeConnectionPointSlots()
{
	vector<int> freeSlots;
	// Check for unoccupied slots
	for (unsigned int i = 0; i < m_connectedPieces.size(); i++)
	{
		if ( !m_connectedSlotsOccupied[i] )
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

const LevelPiece* LevelPiece::getConnectedPiece( int p_inSlot ) const
{
	return m_connectedPieces[p_inSlot];
}

void LevelPiece::setConnectedPiece( int p_inSlot, LevelPiece* p_transform )
{
	m_connectedPieces[p_inSlot] = p_transform;
	if (p_transform)
		m_connectedSlotsOccupied[p_inSlot] = true;
	else
		m_connectedSlotsOccupied[p_inSlot] = false;
}

bool LevelPiece::connectTo( LevelPiece* p_targetPiece, int p_targetSlot )
{
	// TODO: ? Checking collision before pieces are created to reduce overhead?

	// To prevent issues with scale, fetch this scale temporarily
	AglVector3 tempScale = m_transform->getScale();

	 //Rotate the connection point arbitrarily around its forward!
	 //NOTE: Should be enabled again shortly.
	Transform transform = m_connectionPoints[0];
	transform.setRotation(transform.getRotation() 
		* AglQuaternion::constructFromAxisAndAngle(AglVector3::forward(), (rand() % 360) * 3.1415f / 180.0f));
	m_connectionPoints[0] = transform;

	// 1) Transform this piece and all its connection points with the inverse matrix of the
	// used this-connector.
	m_transform->setMatrix(m_transform->getMatrix() * m_connectionPoints[0].getMatrix().inverse());
	m_transform->setScale(tempScale);
	updateConnectionPoints();

	// 2) Flip forward vector using target connector and create a temp matrix.
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

	// 3) Transform this piece and connection points with target piece connector matrix or blä.
	m_transform->setMatrix( m_transform->getMatrix() * temp.getMatrix() );
	//m_transform->setMatrix( m_transform->getMatrix() * mat );
	m_transform->setScale(temp.getScale());

	// NOTE: This is most likely a temporary solution, since the bounding sphere is going
	// to be ridiculously large if the scale is altered.
	m_transform->setScale(AglVector3::one());
	updateConnectionPoints();

	// Set this connection to be occupied!
	p_targetPiece->setConnectedPiece(p_targetSlot, this);
	setConnectedPiece(0, p_targetPiece);

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

int LevelPiece::getMaxChildCount() const
{
	return m_connectedPieces.size();
}

bool LevelPiece::isChildSlotOccupied( int p_inSlot ) const
{
	return m_connectedSlotsOccupied[p_inSlot];
}

int LevelPiece::getPieceId() const
{
	return m_pieceId;
}

void LevelPiece::setPieceId( int p_id )
{
	m_pieceId = p_id;
}

int LevelPiece::getGate( int p_inSlot ) const
{
	return m_gates[p_inSlot];
}

void LevelPiece::setGate( int p_inSlot, int p_id )
{
	m_gates[p_inSlot] = p_id;
}

const vector<LevelPiece*>& LevelPiece::getChildren() const
{
	return m_children;
}

void LevelPiece::addChild( LevelPiece* p_piece )
{
	m_children.push_back(p_piece);
	p_piece->m_parent = this; // Wiahkm priavet member access
}

const LevelPiece* LevelPiece::getParent() const
{
	return m_parent;
}

