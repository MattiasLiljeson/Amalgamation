#pragma once
#include <string>
#include <fstream>
#include <map>
#include <AssemblageHelper.h>
#include <Recipe.h>
#include <EntitySystem.h>
#include "EntityCreationPacket.h"
#include <TcpClient.h>
#include <TcpServer.h>
#include "ParticleSystemsComponent.h"
#include "GradientMapping.h"
#include "SpawnExplosionPacket.h"
#include "Transform.h"
#include "ShipModule.h"

class Entity;
class EntityWorld;
class AssemblageReader;
class LevelInfo;

using namespace std;

// =======================================================================================
//                                      EntityFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief This is a native ES EntitySystem subclass which creates Entities from recipes 
/// which are loaded from assemblage definition files (.asd). 
///        
///	\author		Mattias Liljeson
/// 
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class EntityFactory : public EntitySystem
{
public:
	EntityFactory(TcpClient* p_client, TcpServer* p_server);
	~EntityFactory();

	///-----------------------------------------------------------------------------------
	/// Read assemblage definition file and create a recipe from it which will be stored
	/// locally and used when queried for an Entity. If a recipe already exists it will be
	/// replaced by this new one.
	/// \param p_filePath Path to the assemblage definition file.
	/// \param out_recipeName Name of the created recipe, if the parameter is not NULL.
	/// \return AssemblageHelper::E_FileStatus Status of the operation. If the assemblage
	/// was loaded successfully this function will return
	///-----------------------------------------------------------------------------------
	AssemblageHelper::E_FileStatus readAssemblageFile( string p_filePath, 
													string* out_recipeName=NULL );

	///-----------------------------------------------------------------------------------
	/// Create an entity by supplying recipe/entity name.
	/// \param p_entityName Name of the entity.
	/// \return Entity* An entity which has been created by the world but not added to it.
	/// NULL if the requested recipe isn't found.
	/// This makes it possible to add last minute components to the entity before it is
	/// released into the wild. In order to manipulate already created components, a system
	/// is required.
	///-----------------------------------------------------------------------------------
	Entity* entityFromRecipe( const string& p_entityName );


	///-----------------------------------------------------------------------------------
	/// Combination of readAssemblageFile and entityFromRecipe, will only read assemblage
	/// file if not already read.
	/// \param p_entityName
	/// \param p_filePath
	/// \return Entity*
	///-----------------------------------------------------------------------------------
	Entity* entityFromRecipeOrFile( const string& p_entityName, string p_filePath );

	Entity* entityFromPacket(EntityCreationPacket p_packet, AglMatrix* p_spawnPoint = NULL);

	void createExplosion(const SpawnExplosionPacket& p_packet);

public:
	// Tesla coil
	Entity* createTeslaCoilModuleClient(EntityCreationPacket p_packet);
	Entity* createTeslaCoilModuleServer(EntityCreationPacket p_packet);
	Entity* createTeslaEffectPieceClient(AglVector3 p_forwardScale,
		float p_thicknessFactor, AglQuaternion p_rotation, AglVector3 p_sourcePosition,
		string p_meshName);

	// Anomaly accelerator
	Entity* createAnomalyModuleClient(EntityCreationPacket p_packet);
	Entity* createAnomalyModuleServer(EntityCreationPacket p_packet);
	Entity* createAnomalyBombClient(EntityCreationPacket p_packet);
	Entity* createAnomalyBombServer(Transform* p_transform,
		AglVector3 p_moduleVelocity, ShipModule* p_module);
	Entity* createAnomalyPieces(int p_parentIndex);

	AglVector4 getPlayersFirstGradientLevel() const;
	AglVector4 getPlayersSecondGradientLevel() const;

private:
	//Ship
	Entity* createShipEntityClient(EntityCreationPacket p_packet);
	Entity* createShipEntityServer(EntityCreationPacket p_packet);

	//Modules
	Entity* createMineLayerClient(EntityCreationPacket p_packet);
	Entity* createMineLayerServer(EntityCreationPacket p_packet);
	Entity* createRocketLauncherClient(EntityCreationPacket p_packet);
	Entity* createRocketLauncherServer(EntityCreationPacket p_packet);

	Entity* createMinigunClient(EntityCreationPacket p_packet);
	Entity* createMinigunServer(EntityCreationPacket p_packet);
	Entity* createSpeedBoosterClient(EntityCreationPacket p_packet);
	Entity* createSpeedBoosterServer(EntityCreationPacket p_packet);

	Entity* createModuleClient(EntityCreationPacket p_packet);
	Entity* createModuleServer(EntityCreationPacket p_packet);

	//Laser Sight
	Entity* createLaserSightClient(EntityCreationPacket p_packet);
	Entity* createLaserSightServer(EntityCreationPacket p_packet);

	//Particle System
	Entity* createParticleSystemClient(EntityCreationPacket p_packet);
	Entity* createParticleSystemServer(EntityCreationPacket p_packet);

	//Selection Sphere
	Entity* createSelectionSphereClient(EntityCreationPacket p_packet);
	Entity* createSelectionSphereServer(EntityCreationPacket p_packet);

	//Rocket
	Entity* createRocketClient(EntityCreationPacket p_packet);
	Entity* createRocketServer(EntityCreationPacket p_packet);

	//Mine
	Entity* createMineClient(EntityCreationPacket p_packet);
	Entity* createMineServer(EntityCreationPacket p_packet);

	//Shield
	Entity* createShieldClient(EntityCreationPacket p_packet);
	Entity* createShieldServer(EntityCreationPacket p_packet);

	//Other - Level generation goes here for now.
	Entity* createOtherClient(EntityCreationPacket p_packet);
	Entity* createOtherServer(EntityCreationPacket p_packet);

	void createHighlightParticleEmitter( ParticleSystemsComponent* p_emitters,
		AglVector3 p_spawnPosition, AglVector3 p_spawnDirection, int p_desiredIndex );

private:
	map<string, Recipe*> m_entityRecipes;

	TcpClient* m_client;
	TcpServer* m_server;

private:
	vector<GradientMapping> m_gradientColors;
};

