#include "EntityFactory.h"

#include "AudioBackendSystem.h"
#include "AudioListener.h"
#include "BodyInitData.h"
#include "CameraInfo.h"
#include "ConnectionPointSet.h"
#include "Extrapolate.h"
#include "GameplayTags.h"
#include "InterpolationComponent.h"
#include "LightBlinker.h"
#include "LightsComponent.h"
#include "LookAtEntity.h"
#include "MineLayerModule.h"
#include "NetworkSynced.h"
#include "ParticleEmitters.h"
#include "ParticleRenderSystem.h"
#include "PhysicsBody.h"
#include "PickComponent.h"
#include "PlayerCameraController.h"
#include "PlayerScore.h"
#include "PlayerState.h"
#include "PositionalSoundSource.h"
#include "RenderInfo.h"
#include "RocketLauncherModule.h"
#include "ShipEditController.h"
#include "ShipFlyController.h"
#include "Transform.h"
#include "Vibration.h"
#include <AssemblageException.h>
#include <AssemblageReader.h>
#include <Entity.h>
#include <time.h>
#include "ShipModule.h"
#include "ShieldModule.h"
#include "LoadMesh.h"
#include "LoadMeshSystemClient.h"
#include "EntityParent.h"
#include "ShieldPlate.h"

#define FORCE_VS_DBG_OUTPUT

EntityFactory::EntityFactory(TcpClient* p_client, TcpServer* p_server)
	: EntitySystem( SystemType::EntityFactory ) 
{
	m_client = p_client;
	m_server = p_server;
}

EntityFactory::~EntityFactory()
{
	map<string, Recipe*>::iterator it;

	for( it = m_entityRecipes.begin(); it != m_entityRecipes.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}
	m_entityRecipes.clear();
}

AssemblageHelper::E_FileStatus EntityFactory::readAssemblageFile( string p_filePath )
{
	Recipe* newRecipe = NULL;
	AssemblageReader reader;
	AssemblageHelper::E_FileStatus status = reader.readAssemblageFile( &newRecipe, p_filePath );

	// The file was parsed correctly and a entity was defined in it
	if( status == AssemblageHelper::FileStatus_OK ||
		status == AssemblageHelper::FileStatus_END_OF_FILE )
	{
		if( newRecipe != NULL )
		{
			// Delete previous recipe with the same name, it existing.
			delete m_entityRecipes[newRecipe->getName()];
			m_entityRecipes[newRecipe->getName()] = newRecipe;
		}
	}
	else if (status==AssemblageHelper::FileStatus_FILE_NOT_FOUND)
	{
		throw AssemblageException("Error reading assemblage! "+p_filePath,__FILE__,
			__FUNCTION__,__LINE__);
	}

	return status;
}

Entity* EntityFactory::entityFromRecipe( const string& p_entityName )
{
	Entity* meal = NULL;

	map<string, Recipe*>::iterator it = m_entityRecipes.find( p_entityName );
	if( it != m_entityRecipes.end())
	{
		meal = m_world->createEntity();
		it->second->cook( meal );
		meal->setName( p_entityName );
	}

	return meal;
}
Entity* EntityFactory::entityFromPacket(EntityCreationPacket p_packet, AglMatrix* p_spawnPoint)
{
	EntityType::EntityEnums type = static_cast<EntityType::EntityEnums>(p_packet.entityType);

	Entity* e = NULL;

	if (type == EntityType::Ship)
	{
		if (m_client)
			e = createShipEntityClient(p_packet);
		else
			e = createShipEntityServer(p_packet);
	}
	else if (type == EntityType::MineLayerModule)
	{
		if (m_client)
			e = createMineLayerClient(p_packet);
		else
			e = createMineLayerServer(p_packet);
	}
	else if (type == EntityType::RocketLauncherModule)
	{
		if (m_client)
			e = createRocketLauncherClient(p_packet);
		else
			e = createRocketLauncherServer(p_packet);
	}
	else if (type == EntityType::BoosterModule)
	{
		if (m_client)
			e = createSpeedBoosterClient(p_packet);
		else
			e = createSpeedBoosterServer(p_packet);
	}
	else if (type == EntityType::MinigunModule)
	{
		if (m_client)
			e = createMinigunClient(p_packet);
		else
			e = createMinigunServer(p_packet);
	}
	else if (type == EntityType::ShieldModule)
	{
		if (m_client)
			e = createShieldClient(p_packet);
		else
			e = createShieldServer(p_packet);
	}
	else if (type > EntityType::ShipModuleStart && type < EntityType::EndModule)
	{
		if (m_client)
			e = createModuleClient(p_packet);
		else
			e = createModuleServer(p_packet);
	}
	else if (type == EntityType::LaserSight)
	{
		if (m_client)
			e = createLaserSightClient(p_packet);
		else
			e = createLaserSightServer(p_packet);
	}
	else if (type == EntityType::ParticleSystem)
	{
		if (m_client)
			e = createParticleSystemClient(p_packet);
		else
			e = createParticleSystemServer(p_packet);
	}
	else if (type == EntityType::SelectionSphere)
	{
		if (m_client)
			e = createSelectionSphereClient(p_packet);
		else
			e = createSelectionSphereServer(p_packet);
	}
	else if (type == EntityType::Mine)
	{
		if (m_client)
			e = createMineClient(p_packet);
		else
			e = createMineServer(p_packet);
	}
	else if (type == EntityType::Rocket)
	{
		if (m_client)
			e = createRocketClient(p_packet);
		else
			e = createRocketServer(p_packet);
	}
	else if (type == EntityType::Other)
	{
		if (m_client)
			e = createOtherClient(p_packet);
		else
			e = createOtherServer(p_packet);
	}

	if (e)
	{
		if (p_spawnPoint)
		{
			BodyInitData* init = static_cast<BodyInitData*>(e->getComponent(ComponentType::BodyInitData));
			if (init)
			{
				AglMatrix::matrixToComponents(*p_spawnPoint, init->m_scale, init->m_orientation, init->m_position);
			}
			else
			{
				Transform* transform = static_cast<Transform*>(e->getComponent(ComponentType::Transform));
				if (transform)
				{
					transform->setMatrix(*p_spawnPoint);
				}
			}
		}
		return e;
	}
	else
	{
		DEBUGPRINT(("Network Warning: Received unknown entity type from server!\n"));
		return NULL;
	}
}

Entity* EntityFactory::createShipEntityClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "ClientShip", "Assemblages/ClientShip.asd");

	// Add network dependent components
	Transform* transform = new Transform( p_packet.translation, p_packet.rotation, 
		p_packet.scale);
	entity->addComponent( ComponentType::Transform, transform );

	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, EntityType::Ship));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

	Component* component = NULL; // for temp usage

	/************************************************************************/
	/* Check if the owner is the same as this client.						*/
	/************************************************************************/
	if(m_client->getId() == p_packet.owner)
	{
		component = new ShipFlyController(3.0f, 100.0f);
		entity->addComponent( ComponentType::ShipFlyController, component );

		component = new ShipEditController();
		entity->addComponent( ComponentType::ShipEditController, component);

		entity->addTag( ComponentType::TAG_ShipFlyMode, new ShipFlyMode_TAG );
	
		// hardcoded
		/*ConnectionPointSet* connectionPointSet = new ConnectionPointSet();
		connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
			AglMatrix::createTranslationMatrix(AglVector3(2.5f, 0, 0))));
		connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
			AglMatrix::createTranslationMatrix(AglVector3(-2.5f, 0, 0))));
		connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
			AglMatrix::createTranslationMatrix(AglVector3(0, 2.5f, 0))));
		entity->addComponent(ComponentType::ConnectionPointSet, connectionPointSet);*/
		// NOTE: (Johan) Moved the audio listener to the ship instead of the camera
		// because it was really weird to hear from the camera. This can of course
		// be changed back if game play fails in this way, but it's at least more
		// convenient for debugging!
		component = new AudioListener();
		entity->addComponent(ComponentType::AudioListener, component);

		/************************************************************************/
		/* This is where the audio listener is created and therefor the master  */
		/* volume is added to Ant Tweak Bar here.								*/
		/************************************************************************/
		/************************************************************************/
		/* Sorry. This breaks the server, so it must be moved out of here when	*/
		/* needed. */
		/************************************************************************/
//		AntTweakBarWrapper::getInstance()->addWriteVariable( 
//			AntTweakBarWrapper::OVERALL,
//			"Master_volume", TwType::TW_TYPE_FLOAT, 
//			static_cast<AudioListener*>(component)->getMasterVolumeRef(),
//			"group=Sound min=0 max=10 step=0.001 precision=3");
	}

	component = new PlayerScore();
	entity->addComponent( ComponentType::PlayerScore, component );
	m_world->addEntity(entity);

	/************************************************************************/
	/* Attach a camera if it's the client's ship!							*/
	/************************************************************************/
	if(p_packet.owner == m_client->getId())
	{
		// add a myShip tag to the ship first!
		entity->addComponent( ComponentType::TAG_MyShip, new MyShip_TAG() );
		Entity* entity = m_world->getEntityManager()->getFirstEntityByComponentType(
			ComponentType::TAG_MainCamera);
		
		entity->addComponent(ComponentType::PlayerCameraController, new PlayerCameraController(90.0f) );
		entity->addComponent(ComponentType::NetworkSynced,
			new NetworkSynced(p_packet.miscData, p_packet.owner, EntityType::PlayerCamera));
		entity->addComponent( ComponentType::PlayerState, new PlayerState );
		//Add a picking ray to the camera so that edit mode can be performed
		entity->addComponent(ComponentType::PickComponent, new PickComponent());
		// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

		m_world->addEntity(entity);
	}
	return entity;
}
Entity* EntityFactory::createShipEntityServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createMineLayerClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "MineLayer","Assemblages/Modules/MineLayer/ClientMineLayer.asd" );

	// Add network dependent components
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
		
	// entity->addComponent(ComponentType::InterpolationComponent, new InterpolationComponent());

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createMineLayerServer(EntityCreationPacket p_packet)
{
	AssemblageHelper::E_FileStatus status = readAssemblageFile( "Assemblages/Modules/MineLayer/ServerMineLayer.asd" );
	Entity* entity = entityFromRecipe( "ServerMineLayer" );
	entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::MineLayerModule));
	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createRocketLauncherClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "RocketLauncher", "Assemblages/Modules/RocketLauncher/ClientRocketLauncher.asd" );

	// Add network dependent components
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));

	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createRocketLauncherServer(EntityCreationPacket p_packet)
{
	AssemblageHelper::E_FileStatus status = readAssemblageFile( "Assemblages/Modules/RocketLauncher/ServerRocketLauncher.asd" );
	Entity* entity = entityFromRecipe( "RocketLauncher" );									 
//	entity->addComponent(ComponentType::RocketLauncherModule, new RocketLauncherModule(AglVector3(0, 0, 0), AglVector3(0, 0, 1)));
	entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::RocketLauncherModule));
	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createMinigunClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "ClientMinigun", "Assemblages/Modules/Minigun/ClientMinigun.asd" );

	// Add network dependent components
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

	//AglParticleSystemHeader header;
	//header.particleSize = AglVector2(2, 2);
	//header.spawnFrequency = 10;
	//header.spawnSpeed = 5.0f;
	//header.spread = 0.0f;
	//header.fadeOutStart = 2.0f;
	//header.fadeInStop = 0.0f;
	//header.particleAge = 2;
	//header.maxOpacity = 1.0f;
	//header.color = AglVector4(0, 1, 0, 1.0f);
	//header.alignmentType = AglParticleSystemHeader::OBSERVER;

	//ParticleEmitters* psComp = new ParticleEmitters();
	//psComp->addParticleSystem( header );
	//entity->addComponent( psComp );
	//int particleSysIdx = psComp->addParticleSystem( ParticleSystemData( header, updateData, "minigun" ) );
	
	m_world->addEntity( entity );
	return entity;
}
Entity* EntityFactory::createMinigunServer(EntityCreationPacket p_packet)
{
	return NULL;
}
Entity* EntityFactory::createSpeedBoosterClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "SpeedBooster", "Assemblages/SpeedBooster.asd"  );

	// Add network dependent components
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createSpeedBoosterServer(EntityCreationPacket p_packet)
{
	return NULL;
}
Entity* EntityFactory::createModuleClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	entity = entityFromRecipeOrFile( "DebugCube", "Assemblages/DebugCube.asd" );

	// Add network dependent components
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createModuleServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createLaserSightClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	if (p_packet.meshInfo == 1)
		entity = entityFromRecipeOrFile( "DebugSphere", "Assemblages/DebugSphere.asd" );
	else
		entity = entityFromRecipeOrFile( "DebugCube", "Assemblages/DebugCube.asd" );

	// Add network dependent components
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createLaserSightServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createParticleSystemClient(EntityCreationPacket p_packet)
{
	// DEPRECATED! //ML
	//AglParticleSystemHeader h;
	//if (p_packet.meshInfo == 0)
	//{
	//	h.particleSize = AglVector2(2, 2);
	//	h.spawnFrequency = 10;
	//	h.spawnSpeed = 5.0f;
	//	h.spread = 0.0f;
	//	h.fadeOutStart = 2.0f;
	//	h.fadeInStop = 0.0f;
	//	h.particleAge = 2;
	//	h.maxOpacity = 1.0f;
	//	h.color = AglVector4(0, 1, 0, 1.0f);
	//	h.alignmentType = AglParticleSystemHeader::OBSERVER;
	//}
	//else
	//{
	//	h.particleSize = AglVector2(1.0f, 1.0f);
	//	h.spawnFrequency = 10;
	//	h.spawnSpeed = 0.02;
	//	h.particleAge = 1;
	//	h.spread = 1.0f;
	//	h.fadeOutStart = 0.5f;
	//	h.fadeInStop = 0.5f;
	//	h.particleAge = 1;
	//	h.maxOpacity = 0.5f;
	//	h.color = AglVector4(0, 1.0f, 0.7f, 1.0f);
	//	h.spawnOffset = 4.0f;
	//	h.spawnOffsetType = AglParticleSystemHeader::ONSPHERE;
	//}

	//ParticleRenderSystem* gfx = static_cast<ParticleRenderSystem*>(m_world->getSystem(
	//	SystemType::ParticleRenderSystem ));
	//gfx->addParticleSystem();
	//gfx->addParticleSystem(h, p_packet.networkIdentity);
	return NULL;
}
Entity* EntityFactory::createParticleSystemServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createSelectionSphereClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	if (p_packet.meshInfo == 1)
		entity = entityFromRecipeOrFile( "DebugSphere", "Assemblages/DebugSphere.asd" );
	else
		entity = entityFromRecipeOrFile( "DebugCube", "Assemblages/DebugCube.asd" );

	// Add network dependent components
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createSelectionSphereServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createRocketClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "Rocket","Assemblages/Rocket.asd"  );

	// Add network dependent components
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
	entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());
	entity->addComponent( ComponentType::PositionalSoundSource, new PositionalSoundSource(
		TESTSOUNDEFFECTPATH, "Missile_Flight.wav" ));
	m_world->addEntity(entity);
	static_cast<AudioBackendSystem*>(m_world->getSystem(SystemType::AudioBackendSystem))->
		playPositionalSoundEffect(TESTSOUNDEFFECTPATH, "Missile_Start.wav",
		p_packet.translation);
	return entity;
}
Entity* EntityFactory::createRocketServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createMineClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "Mine", "Assemblages/Mine.asd" );

	// Add network dependent components
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());
	entity->addComponent( ComponentType::PositionalSoundSource, new PositionalSoundSource(
		TESTSOUNDEFFECTPATH, "Mine_Blip.wav") );

	Vibration* v = new Vibration(100.0f,10.0f,40.0f);
	v->enabled = true;
	entity->addComponent( ComponentType::Vibration, v );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createMineServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createShieldClient(EntityCreationPacket p_packet)
{
	Entity* shieldEntity = m_world->createEntity();
	// Add network dependent components
	Transform* transform = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	shieldEntity->addComponent( ComponentType::Transform, transform );
	shieldEntity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	shieldEntity->addComponent(ComponentType::LoadMesh, new LoadMesh("shield_module.agl"));
	shieldEntity->addComponent(ComponentType::ShieldModule, new ShieldModule());
	m_world->addEntity(shieldEntity);

	for(unsigned int i=0; i<60; i++)
	{
		float spawnX, spawnY;
		circularRandom(&spawnX, &spawnY);
		Entity* entity = m_world->createEntity();
		AglVector3 spawnPoint = AglVector3(0, 7.0f, 0); // Replace with real spawn point.
		float radius = 10.0f;
		AglVector3 position = spawnPoint + AglVector3(radius * spawnX, 0, radius * spawnY);
		position.normalize();
		AglQuaternion plateRotation = AglQuaternion::identity();
		position = position * spawnPoint.length();
		float plateScale = 1.0f;
		Transform* plateTransform = new Transform(position, plateRotation,
			AglVector3(plateScale, plateScale, plateScale));
		entity->addComponent(plateTransform);
		entity->addComponent(new LoadMesh("shield_plate.agl"));
		entity->addComponent(new EntityParent(shieldEntity->getIndex(),
			plateTransform->getMatrix()));
		entity->addComponent(new ShieldPlate(0.2f + 0.8f * (float)rand()/(float)RAND_MAX));
		m_world->addEntity(entity);
	}

	return shieldEntity;
}
Entity* EntityFactory::createShieldServer(EntityCreationPacket p_packet)
{
	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation,
		p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	entity->addComponent( ComponentType::PhysicsBody, new PhysicsBody() );
	entity->addComponent( ComponentType::BodyInitData, 
		new BodyInitData(p_packet.translation,
		p_packet.rotation,
		p_packet.scale, AglVector3(0, 0, 0), 
		AglVector3(0, 0, 0), 0, 
		BodyInitData::DYNAMIC, 
		BodyInitData::SINGLE, false));
	entity->addComponent(ComponentType::LoadMesh, new LoadMesh("shield_module.agl"));
	entity->addComponent(ComponentType::ShipModule, new ShipModule());
	entity->addComponent(ComponentType::ShieldModule, new ShieldModule());
	entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(
		entity->getIndex(), -1, EntityType::ShieldModule));
	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createOtherClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	if (p_packet.isLevelProp)
	{
		// meshId = gfxBackend->getMeshId(m_levelPieceMapping.getModelFileName(p_packet.meshInfo));
		// changed during refactoring by Jarl 30-1-2013
		// use an assemblage, like this:
		entity = entityFromRecipeOrFile( "DebugSphere", "Assemblages/DebugSphere.asd" );
	}
	else	
	{
		if (p_packet.meshInfo == 1)
			entity = entityFromRecipeOrFile( "DebugSphere", "Assemblages/DebugSphere.asd" );
		else
			entity = entityFromRecipeOrFile( "DebugCube", "Assemblages/DebugCube.asd" );

	}
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createOtherServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}

void EntityFactory::circularRandom( float* p_spawnX, float* p_spawnY )
{
//	float x = 0.0f;
//	float y = 0.0f;
//	do
//	{
//		x = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
//		y = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
//	} while( x * x + y * y > 1.0f );
//	*p_spawnX = x;
//	*p_spawnY = y;

	// NOTE: This was almost right. Going for the simpler for now though.
	float x = 0.0f;
	float y = 0.0f;
	do
	{
		float x_rand = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
		float y_rand = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
		bool x_negative = x_rand < 0;
		bool y_negative = y_rand < 0;
		x_rand = x_rand * x_rand;
		y_rand = y_rand * y_rand;
		if(x_negative)
			x_rand = -x_rand;
		if(y_negative)
			y_rand = -y_rand;
		x = x_rand;
		y = y_rand;
	} while( x * x + y * y > 1.0f );
	*p_spawnX = x;
	*p_spawnY = y;
}

Entity* EntityFactory::entityFromRecipeOrFile( const string& p_entityName, string p_filePath )
{
	// performance test
	clock_t init, final;
	init=clock();
	//
	Entity* entity = entityFromRecipe(p_entityName);
	if (entity==NULL)
	{	
		readAssemblageFile( p_filePath );
		entity = entityFromRecipe( p_entityName );
	}
	final=clock()-init;
	DEBUGPRINT(( ("\n"+p_entityName+" constructed in "+toString((double)final / ((double)CLOCKS_PER_SEC))+" seconds.\n").c_str() ));
	return entity;
}
