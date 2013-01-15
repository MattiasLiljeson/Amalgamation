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
class TcpServer;

using namespace std;
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
	LevelGenSystem(GraphicsBackendSystem* p_graphicsBackend, TcpServer* p_server);
	virtual ~LevelGenSystem();

	void setPieceTypes(vector<ConnectionPointCollection> p_pieceTypes,
						vector<AglMeshHeader> p_aglMeshHeaders);

	const vector<LevelPiece*>& getGeneratedLevelPieces() const;
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

	GraphicsBackendSystem*	m_graphicsBackend;
	TcpServer*				m_server;
};
