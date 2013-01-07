#pragma once
#define WINAPI

#include <EntitySystem.h>
#include <vector>
#include <map>

class Transform;

using namespace std;
// =======================================================================================
//                                      TransformNode
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # TransformNode
/// Detailed description.....
/// Created on: 3-1-2013 
///---------------------------------------------------------------------------------------
struct TransformNode
{
	int						type;
	TransformNode*			parent;
	Transform*				transform;
	vector<TransformNode*>	children;

	static void connect(TransformNode* p_parent, TransformNode* p_child, int p_childSlot);
};

// =======================================================================================
//                                      LevelGenSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LevelGenSystem
/// Detailed description.....
/// Created on: 3-1-2013 
///---------------------------------------------------------------------------------------

class LevelGenSystem : public EntitySystem
{
public:
	LevelGenSystem();
	virtual ~LevelGenSystem();

	void initialize();
protected:

private:
	void getFreeConnectPointSlots(TransformNode* p_piece, vector<int>& out_slots);

	int popIntVector(vector<int>& p_vector);

	TransformNode*	createTransformNodeFromType(int p_type);
	void			deleteTransformNodeRecursive(TransformNode* p_node);

	void createAndAddEntity(int p_type, Transform* p_transform);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(TransformNode* p_targetPiece, 
								vector<TransformNode*>& out_pieces);

	void connectPieces(Transform* p_sourcePiece, Transform* p_sourceConnector,
						Transform* p_otherPiece, Transform* p_otherConnector);
	void addEndPlug(Transform* p_atConnector);

	void debugPrintTransformNode(TransformNode* p_node, stringstream& p_stream, 
								int currentDepth);
	void debugPrintTransformTree();

	map<Transform*, TransformNode*> m_transformHierarchy;
	
	Transform* m_invalidAttachment;
	TransformNode* m_rootTransform;
};
