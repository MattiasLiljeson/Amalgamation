#pragma once

#include <EntitySystem.h>
#include <vector>
#include "LevelPieceFileMapping.h"
#include <ModelBaseFactory.h>
#include <string>

class Transform;
class LevelPiece;
class TcpServer;
class EntityFactory;
struct ModelResource;
struct AglVector3;
class LevelInfo;

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

	void processEntities( const vector<Entity*>& p_entities );

	void clearGeneratedData();

	void run();
	void createLevelEntities();
	const AglVector3& getWorldMin() const;
	const AglVector3& getWorldMax() const;
	vector<LevelPiece*> getGeneratedPieces() const;
protected:

private:
	void preloadLevelGenRecipeEntity(const string& p_filePath);
	void preloadLevelPieceRecipes();

	int popIntVector(vector<int>& p_vector);
	
	//void createAndAddEntity(int p_type, Transform* p_transform, const AglOBB& p_obb);
	Entity* createEntity(LevelPiece* p_piece, int p_pieceInstanceId);
	 // Used to generate a bounding sphere around the chamber.
	Entity* createDebugSphereEntity(LevelPiece* p_piece);

	void generateLevelPieces(int p_maxDepth, bool p_doRandomStartRotation);
	void generatePiecesOnPiece(LevelPiece* p_targetPiece, 
								vector<LevelPiece*>& out_pieces, int p_generation);

	void calculatePieceCollision(vector<ModelResource*>* p_pieceMesh);

	void addEndPlug(Transform* p_atConnector);

	void updateWorldMinMax(AglOBB& boundingVolume);

	virtual void inserted( Entity* p_entity );
	virtual void removed( Entity* p_entity );

	//ModelBaseFactory m_unmanagedModelFactory;
	EntityFactory*	m_entityFactory;
	LevelInfo* m_levelInfo; // This is a component, be cautious!

	vector<ModelResource*>	m_modelResources;
	LevelPieceFileMapping	m_modelFileMapping;

	vector<LevelPiece*> m_generatedPieces;

	TcpServer*				m_server;

	AglVector3 m_worldMin;
	AglVector3 m_worldMax;
};
