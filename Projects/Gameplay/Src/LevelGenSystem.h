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
	Transform*			parent;
	Transform*			transform;
	vector<Transform*>	children;
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
	void getFreeAttachments(Transform* p_piece, vector<Transform*>& out_attachments);

	Transform* popTransformVector(vector<Transform*>& p_vector);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(Transform* p_targetPiece, vector<Transform*>& out_pieces);
	void connectPieces(Transform* p_sourcePiece, Transform* p_sourceConnector,
						Transform* p_otherPiece, Transform* p_otherConnector);
	void addEndPlug(Transform* p_atConnector);

	void debugPrintTransformNode(TransformNode* p_node, stringstream& p_stream, 
								int currentDepth);
	void debugPrintTransformTree();

	map<Transform*, TransformNode*> m_transformHierarchy;

	Transform* m_invalidAttachment;
	Transform* m_rootTransform;
};
