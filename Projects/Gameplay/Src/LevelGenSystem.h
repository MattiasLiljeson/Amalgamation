#pragma once

#include <EntitySystem.h>
#include <vector>
#include "LevelPieceFileMapping.h"
#include <ModelBaseFactory.h>

class Transform;
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
	LevelGenSystem(TcpServer* p_server);
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
	
	//void createAndAddEntity(int p_type, Transform* p_transform, const AglOBB& p_obb);
	Entity* createEntity(LevelPiece* p_piece);

	void generateLevelPieces(int p_maxDepth);
	void generatePiecesOnPiece(LevelPiece* p_targetPiece, 
								vector<LevelPiece*>& out_pieces);

	void addEndPlug(Transform* p_atConnector);

	void updateWorldMinMax(AglOBB& boundingVolume);

	ModelBaseFactory m_unmanagedModelFactory;

	vector<ModelResource*>	m_modelResources;
	LevelPieceFileMapping	m_modelFileMapping;

	vector<LevelPiece*> m_generatedPieces;

	TcpServer*				m_server;

	AglVector3 m_worldMin;
	AglVector3 m_worldMax;
};
