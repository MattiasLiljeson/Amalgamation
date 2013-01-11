#include "LevelPiece.h"

#include <ConnectionPointCollection.h>
#include "Transform.h"
#include <cstdlib>

LevelPiece::LevelPiece( ConnectionPointCollection* p_connectionPoints,
					   AglMeshHeader* p_meshHeader, 
					   Transform* p_transform )
{
	m_localSpaceConnectionPoints	= p_connectionPoints;
	m_transform			= p_transform;
	m_meshHeader		= p_meshHeader;

	int maxChildCount = m_localSpaceConnectionPoints->m_collection.size();
	m_childSlotsOccupied.resize(maxChildCount);
	m_children.resize(maxChildCount, nullptr);
	m_connectionPoints.resize(maxChildCount);

	updateConnectionPoints();
}

vector<int> LevelPiece::findFreeConnectionPointSlots()
{
	vector<int> freeSlots;
	// Check for unoccupied slots
	for (int i = 0; i < m_children.size(); i++)
	{
		if ( !m_childSlotsOccupied[i] )
			freeSlots.push_back(i);
	}
	return freeSlots;
}

Transform* LevelPiece::getTransform()
{
	return m_transform;
}

int LevelPiece::getMeshId()
{
	return m_localSpaceConnectionPoints->m_meshId;
}

Transform* LevelPiece::getChild( int p_inSlot )
{
	return m_children[p_inSlot];
}

void LevelPiece::setChild( int p_inSlot, Transform* p_transform )
{
	m_children[p_inSlot]			= p_transform;
	m_childSlotsOccupied[p_inSlot]	= true;
}

void LevelPiece::connectTo( LevelPiece* p_targetPiece, int p_targetSlot )
{
	// From Proto \\ Jarl & Alex

	// To prevent issues with scale, fetch this scale temporarily
	AglVector3 tempScale = m_transform->getScale();

	// Rotate the connection point arbitrarily around its forward!
	Transform transform = m_connectionPoints[0];
	transform.setRotation(transform.getRotation() 
		* AglQuaternion::constructFromAxisAndAngle(transform.getForward(), (rand() % 360) * 3.1415f / 180.0f));
	m_connectionPoints[0] = transform;

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

	//return;
	//// From Proto \\ Anton & Alex

	//// Set the position of this piece to the position of the target, with the offset of
	//// the target connection point and this connection point
	//// connectionPoints are assumed to be in local space relative its parent (piece).
	//// Pieces are assumed to be in global space.

	//AglMatrix thisLocalConnectMat = getLocalConnectionPointMatrix(0);

	//// Target connection point matrix in world space
	//AglMatrix targetGlobalConnectMat = p_targetPiece->getLocalConnectionPointMatrix(p_targetSlot, Space_GLOBAL);
	//AglQuaternion rotation;
	//AglVector3 scale, translation;
	//targetGlobalConnectMat.toComponents(scale, rotation, translation);

	//AglMatrix thisNewGlobalMat = targetGlobalConnectMat * 
	//								thisLocalConnectMat.inverse();

	//// TODO: Fetch new code from sprint 4 that has AglMatrix::GetRotation()
	//thisNewGlobalMat.toComponents(scale, rotation, translation);

	//rotation *= AglQuaternion::constructFromAxisAndAngle(thisLocalConnectMat.GetRight(), 3.141592f);
	//m_transform->setRotation(rotation);
	//

	//// Positioning
	//// -----------
	//// Get distance from this piece and its connection point. This can be found by taking
	//// the length of the connection point's position since it is defined in local space.
	//AglVector3 thisConnectionPointPosition = thisLocalConnectMat.GetTranslation();
	//float length = thisConnectionPointPosition.length();
	//AglVector3 globalTargetConnectionPointPosition = targetGlobalConnectMat.
	//												GetTranslation();

	//// Move this piece using the forward direction of the target point connection
	//m_transform->setTranslation( globalTargetConnectionPointPosition + 
	//							targetGlobalConnectMat.GetForward() * length);

	////m_transform->setTranslation( p_targetPiece->getTransform()->getTranslation() +
	////							targetConnectionPointMatrix.GetTranslation() +
	////							thisConnectionPointMatrix.GetTranslation() );

	//// Set this connection to be occupied!
	//p_targetPiece->setChild(p_targetSlot, m_transform);
	//setChild(0, p_targetPiece->getTransform());
}

AglMatrix LevelPiece::getLocalConnectionPointMatrix( int p_vectorIndex, E_Space p_inSpace/*=Space_LOCAL*/ )
{
	if (p_inSpace == Space_LOCAL)
		return AglMatrix( m_localSpaceConnectionPoints->m_collection[p_vectorIndex].transform );
	else
		return AglMatrix( m_localSpaceConnectionPoints->m_collection[p_vectorIndex].transform ) *
				m_transform->getMatrix();
}

void LevelPiece::updateConnectionPoints()
{
	for (int i = 0; i < m_connectionPoints.size(); i++)
	{
		m_connectionPoints[i] = Transform(
			AglMatrix( m_localSpaceConnectionPoints->m_collection[i].transform ) *
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
