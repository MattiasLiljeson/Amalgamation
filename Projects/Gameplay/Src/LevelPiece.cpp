#include "LevelPiece.h"

#include <ConnectionPointCollection.h>
#include "Transform.h"

LevelPiece::LevelPiece( ConnectionPointCollection* p_connectionPoints,
					   Transform* p_transform )
{
	m_connectionPoints	= p_connectionPoints;
	m_transform			= p_transform;

	int maxChildCount = m_connectionPoints->m_collection.size();
	m_childSlotsOccupied.resize(maxChildCount);
	m_children.resize(maxChildCount, nullptr);
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
	return m_connectionPoints->m_meshId;
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
	// Set the position of this piece to the position of the target, with the offset of
	// the target connection point and this connection point
	AglMatrix targetConnectionPointMatrix = p_targetPiece->
												getConnectionPointMatrix(p_targetSlot);
	AglMatrix thisConnectionPointMatrix = getConnectionPointMatrix(0);

	// Target connection point matrix in world space
	// TODO: Continue here, merge from sprint4
	//AglMatrix globalTargetConnectionPointMatrix = targetConnectionPointMatrix *
	//												p_targetPiece->getTransform()->

	m_transform->setTranslation( p_targetPiece->getTransform()->getTranslation() +
								targetConnectionPointMatrix.GetTranslation() +
								thisConnectionPointMatrix.GetTranslation() );
}

AglMatrix LevelPiece::getConnectionPointMatrix( int p_vectorIndex )
{
	return AglMatrix( m_connectionPoints->m_collection[p_vectorIndex].transform );
}
