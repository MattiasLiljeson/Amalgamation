#pragma once
#define WINAPI

#include <EntitySystem.h>
#include <ConnectionPointCollection.h>
#include <AglMesh.h>
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

	void setPieceTypes(vector<ConnectionPointCollection> p_pieceTypes,
						vector<AglMeshHeader> p_aglMeshHeaders);

	void initialize();

	void run();
protected:

private:
	int popIntVector(vector<int>& p_vector);

	int	getRandomPieceType();

	void createAndAddEntity(int p_type, Transform* p_transform);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(LevelPiece* p_targetPiece, 
								vector<LevelPiece*>& out_pieces);

	void addEndPlug(Transform* p_atConnector);

	vector<ConnectionPointCollection> m_pieceTypes;
	vector<AglMeshHeader> m_meshHeaders;
	vector<LevelPiece*> m_generatedPieces;

	GraphicsBackendSystem* graphicsBackend;
};
