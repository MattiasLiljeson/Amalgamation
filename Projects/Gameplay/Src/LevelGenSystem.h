#pragma once
#define WINAPI

#include <EntitySystem.h>
#include <ConnectionPointCollection.h>
#include <vector>
#include <map>

class Transform;
class GraphicsBackendSystem;
class LevelPiece;

using namespace std;

class TransformNode
{

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

	void setPieceTypes(vector<ConnectionPointCollection> p_pieceTypes);

	void initialize();

	void run();
protected:

private:
	int popIntVector(vector<int>& p_vector);

	TransformNode*	createTransformNodeFromType(int p_type);
	void			deleteTransformNodeRecursive(TransformNode* p_node);
	int				getRandomPieceType();

	void createAndAddEntity(int p_type, Transform* p_transform);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(LevelPiece* p_targetPiece, 
								vector<LevelPiece*>& out_pieces);

	void connectPieces(Transform* p_sourcePiece, Transform* p_sourceConnector,
						Transform* p_otherPiece, Transform* p_otherConnector);
	void addEndPlug(Transform* p_atConnector);

	void debugPrintTransformNode(TransformNode* p_node, stringstream& p_stream, 
								int currentDepth);
	void debugPrintTransformTree();

	vector<ConnectionPointCollection> m_pieceTypes;

	map<Transform*, TransformNode*> m_transformHierarchy;
	
	Transform* m_invalidAttachment;
	TransformNode* m_rootTransform;

	GraphicsBackendSystem* graphicsBackend;
};
