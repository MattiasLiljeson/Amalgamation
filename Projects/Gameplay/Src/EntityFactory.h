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
#include "LevelPieceFileMapping.h"
#include "ParticleSystemsComponent.h"

class Entity;
class EntityWorld;
class AssemblageReader;

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
	/// \return AssemblageHelper::E_FileStatus Status of the operation. If the assemblage
	/// was loaded successfully this function will return
	///-----------------------------------------------------------------------------------
	AssemblageHelper::E_FileStatus readAssemblageFile( string p_filePath );

	///-----------------------------------------------------------------------------------
	/// Create an entity by supplying recipe/entity name.
	/// \param p_entityName Name of the entity.
	/// \return Entity* An entity which has been created by the world but not added to it.
	/// This is so that you can make last minute changes to the components and their data
	/// before letting it out in the wild. NULL if the requested recipe isn't found.
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

	//Other
	Entity* createOtherClient(EntityCreationPacket p_packet);
	Entity* createOtherServer(EntityCreationPacket p_packet);

	void circularRandom(float* p_spawnX, float* p_spawnY, bool p_warpCompensation=false );
	void createHighlightParticleEmitter( ParticleSystemsComponent* p_emitters,
		AglVector3 p_spawnPosition, AglVector3 p_spawnDirection );
private:
	map<string, Recipe*> m_entityRecipes;

	TcpClient* m_client;
	TcpServer* m_server;

	LevelPieceFileMapping m_levelPieceMapping;
};

