#pragma once

#include <EntitySystem.h>
#include <ConnectionPointCollection.h>
#include <AglMesh.h>
#include <vector>
#include "LevelPieceFileMapping.h"
#include <ModelBaseUnmanagedFactory.h>

class Transform;
class GraphicsBackendSystem;
class LevelPiece;
class TcpServer;
struct ModelResource;
struct AglVector3;

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

	void initialize();

	void run();
	void createLevelEntities();
	const AglVector3& getWorldMin() const;
	const AglVector3& getWorldMax() const;
	vector<LevelPiece*> getGeneratedPieces() const;
protected:

private:
	int popIntVector(vector<int>& p_vector);
	
	int getMeshFromPieceType(int p_typeId) const;

	//void createAndAddEntity(int p_type, Transform* p_transform, const AglOBB& p_obb);
	Entity* createEntity(LevelPiece* p_piece);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(LevelPiece* p_targetPiece, 
								vector<LevelPiece*>& out_pieces);

	void addEndPlug(Transform* p_atConnector);

	void updateWorldMinMax(AglOBB& boundingVolume);

	ModelBaseUnmanagedFactory m_unmanagedModelFactory;

	vector<ModelResource*>	m_modelResources;
	LevelPieceFileMapping	m_modelFileMapping;

	vector<ConnectionPointCollection> m_pieceTypes;
	vector<AglMeshHeader> m_meshHeaders;
	vector<LevelPiece*> m_generatedPieces;

	GraphicsBackendSystem*	m_graphicsBackend;
	TcpServer*				m_server;

	AglVector3 m_worldMin;
	AglVector3 m_worldMax;
};
