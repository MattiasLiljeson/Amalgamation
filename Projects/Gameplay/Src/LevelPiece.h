#pragma once

#include <vector>
#include <AglMatrix.h>
using namespace std;

class Transform;
struct ConnectionPointCollection;
struct AglMeshHeader;

enum E_Space
{
	Space_LOCAL,
	Space_GLOBAL
};

// =======================================================================================
//                                      LevelPiece
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelPiece
/// Detailed description.....
/// Created on: 9-1-2013 
///---------------------------------------------------------------------------------------

class LevelPiece
{
public:
	LevelPiece(ConnectionPointCollection* p_connectionPoints,
				AglMeshHeader* p_meshHeader, 
				Transform* p_transform);

	AglMatrix	getConnectionPointMatrix(int p_vectorIndex, E_Space p_inSpace = Space_LOCAL);
	vector<int> findFreeConnectionPointSlots();
	Transform*	getTransform();
	void		setTransform();
	int			getMeshId();
	Transform*	getChild(int p_inSlot);
	void		setChild(int p_inSlot, Transform* p_transform);
	void		connectTo(LevelPiece* p_targetPiece, int p_targetSlot);
protected:
private:
	ConnectionPointCollection*	m_localSpaceConnectionPoints;
	AglMeshHeader*				m_meshHeader;
	vector<bool>				m_childSlotsOccupied;
	Transform*					m_transform;
	vector<Transform*>			m_children;
	vector<Transform*>			m_connectionPoints;

};