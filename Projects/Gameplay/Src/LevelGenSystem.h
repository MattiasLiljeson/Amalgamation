#pragma once

#include <EntitySystem.h>
#include <vector>
#include "LevelPieceFileData.h"
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

	void clearGeneratedData();
	int getGeneratedPiecesCount() const;

	void resetState();

	void generateLevel(int p_nrOfPlayers);

	void createLevelEntities();
	const AglVector3& getWorldMin() const;
	const AglVector3& getWorldMax() const;
	vector<LevelPiece*> getGeneratedPieces() const;

	bool isLevelGenerated() const;
protected:

private:
	void preloadLevelGenRecipeEntity(const string& p_filePath);
	void preloadLevelPieceRecipes();

	int popIntVector(vector<int>& p_vector);
	
	//void createAndAddEntity(int p_type, Transform* p_transform, const AglOBB& p_obb);
	Entity* createEntity(LevelPiece* p_piece);
	 // Used to generate a bounding sphere around the chamber.
	Entity* createDebugSphereEntity(LevelPiece* p_piece);

	void generateLevelPieces(int p_maxDepth, bool p_doRandomStartRotation);
	void generatePiecesOnPiece(LevelPiece* p_targetPiece, 
								vector<LevelPiece*>& out_pieces, int p_generation);

	bool tryConnectPieces(LevelPiece* p_target, LevelPiece* p_newPiece, int p_slot);

	void calculatePieceCollision(vector<ModelResource*>* p_pieceMesh);

	void addEndPlugs(LevelPiece* p_atPiece);
	Entity* addEndPlug(Transform* p_atConnector);

	void updateWorldMinMax(AglOBB& boundingVolume);

	virtual void inserted( Entity* p_entity );
	virtual void removed( Entity* p_entity );

	void addResource(ModelResource* p_modelResource);
	bool testLevelMaxSizeHit();

	EntityFactory*	m_entityFactory;
	LevelInfo* m_levelInfo; // This is a component, be cautious!

	vector<ModelResource*>	m_modelResources;
	ModelResource*			m_endPlugModelResource;
	vector<int>				m_sortedResourceIds;	// Contains a list of indices of
													// model resources, sorted in
													// size, with the highest first.
	
	vector<LevelPiece*> m_generatedPieces;

	TcpServer*	m_server;
	bool		m_readyToRun;
	bool		m_hasGeneratedLevel;
	int			m_nrOfPlayers;
	int			m_levelMaxSize; // Max size, specified based on the number of players.
	int			m_currentLevelSize;
	bool		m_useLevelMaxSize;
	bool		m_hasHitLevelMaxSize;

	AglMatrix m_startTransform;

	AglVector3 m_worldMin;
	AglVector3 m_worldMax;
};
