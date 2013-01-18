#pragma once
#define WINAPI

#include <EntitySystem.h>
#include <ConnectionPointCollection.h>
#include <AglMesh.h>
#include <vector>
#include <map>
#include "LevelPieceModelMapping.h"
#include <ModelBaseUnmanagedFactory.h>

class Transform;
class GraphicsBackendSystem;
class LevelPiece;
class TcpServer;
struct ModelResource;

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

	//void setPieceTypes(vector<ConnectionPointCollection> p_pieceTypes,
	//					vector<AglMeshHeader> p_aglMeshHeaders);

	//const vector<LevelPiece*>& getGeneratedLevelPieces() const;
	void initialize();

	void run();
protected:

private:
	int popIntVector(vector<int>& p_vector);
	
	int getMeshFromPieceType(int p_typeId) const;

	//int	getRandomPieceType();

	void createAndAddEntity(int p_type, Transform* p_transform);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(LevelPiece* p_targetPiece, 
								vector<LevelPiece*>& out_pieces);

	void addEndPlug(Transform* p_atConnector);

	ModelBaseUnmanagedFactory m_unmanagedModelFactory;

	vector<ModelResource*>	m_modelResources;
	LevelPieceFileMapping	m_modelFileMapping;

	vector<ConnectionPointCollection> m_pieceTypes;
	vector<AglMeshHeader> m_meshHeaders;
	vector<LevelPiece*> m_generatedPieces;

	GraphicsBackendSystem*	m_graphicsBackend;
	TcpServer*				m_server;
};
