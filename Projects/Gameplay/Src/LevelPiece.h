#pragma once

#include <vector>
#include <AglMatrix.h>
#include <AglOBB.h>
#include <AglBoundingSphere.h>

using namespace std;

class Transform;
struct ConnectionPointCollection;
struct AglMeshHeader;
struct ModelResource;

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
	//LevelPiece(ConnectionPointCollection* p_connectionPoints,
	//			AglMeshHeader* p_meshHeader, 
	//			Transform* p_transform);
	~LevelPiece();

	LevelPiece( int p_typeId, ModelResource* p_modelResource, Transform* p_transform,
				int p_generation);

	void		setPieceId(int p_id);

	void		deleteMainTransform();
	void		updateConnectionPoints();
	AglMatrix	getLocalConnectionPointMatrix(int p_vectorIndex, E_Space p_inSpace = Space_LOCAL);
	AglMatrix	getConnectionPointMatrix(int p_vectorIndex);
	Transform	getConnectionPoint(int p_vectorIndex);
	vector<int> findFreeConnectionPointSlots();
	Transform*  getTransform() const;
	int			getTypeId() const;
	int			getPieceId() const;
	const Transform* getChild(int p_inSlot) const;
	bool		isChildSlotOccupied(int p_inSlot) const;
	int			getMaxChildCount() const;
	void		setChild(int p_inSlot, Transform* p_transform);
	bool		connectTo(LevelPiece* p_targetPiece, int p_targetSlot);
	const AglOBB& getBoundingBox() const;
	const AglBoundingSphere& getBoundingSphere() const;
	const ModelResource* getModelResource() const;
protected:
private:
	void updateBoundingVolumes();

	ModelResource*				m_modelResource;
	int							m_typeId;
	int							m_pieceId;
	int							m_generation;
	//ConnectionPointCollection*	m_localSpaceConnectionPoints;
	//AglMeshHeader*				m_meshHeader;
	vector<bool>				m_childSlotsOccupied;
	Transform*					m_transform;
	vector<Transform*>			m_children;
	vector<Transform>			m_connectionPoints;
	AglOBB						m_boundingBox;
	AglBoundingSphere			m_boundingSphere;
};