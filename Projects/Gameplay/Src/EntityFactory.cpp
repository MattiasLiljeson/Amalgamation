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
#include "ParticleSystemsComponent.h"
#include "ParticleRenderSystem.h"
#include "PhysicsBody.h"
#include "PickComponent.h"
#include "PlayerCameraController.h"
#include "PlayerComponent.h"
#include "PlayerState.h"
#include "SoundComponent.h"
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
#include "ShieldModuleActivationClient.h"
#include "SpeedBoosterModule.h"
#include "ParticleSystemServerComponent.h"
#include "LevelPieceRoot.h"
#include "ParticleSystemEmitter.h"
#include "GradientComponent.h"
#include "LevelInfoLoader.h"
#include "LevelPieceFileData.h"
#include "ConnectionVisualizerSystem.h"
#include "AnomalyAcceleratorModule.h"
#include "InterpolationComponent2.h"
#include "AnomalyBomb.h"
#include "AnomalyBombEffectPiece.h"
#include "CircularMovement.h"
#include <RandomUtil.h>
#include "TeslaCoilModule.h"
#include "TeslaEffectPiece.h"
#include "StaticProp.h"
#include "ShineSpawn.h"
#include "ThrustComponent.h"

#define FORCE_VS_DBG_OUTPUT


EntityFactory::EntityFactory(TcpClient* p_client, TcpServer* p_server)
	: EntitySystem( SystemType::EntityFactory ) 
{
	m_client = p_client;
	m_server = p_server;
	//Player 1 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(47.0f/255.0f,77.0f/255.0f,82.0f/255.0f,255.0f/255.0f),
		AglVector4(104.0f/255.0f,47.0f/255.0f,208.0f/255.0f,255.0f/255.0f)));
	//Player 2 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(47.0f/255.0f,208.0f/255.0f,172.0f/255.0f,1),
		AglVector4(47.0f/255.0f,176.0f/255.0f,208.0f/255.0f,1)
		));
	//Player 3 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(232.0f/255.0f,44.0f/255.0f,12.0f/255.0f,1),
		AglVector4(232.0f/255.0f,116.0f/255.0f,12.0f/255.0f,1)
		));
	//Player 4 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(248.0f/255.0f,220.0f/255.0f,31.0f/255.0f,1),
		AglVector4(210.0f/255.0f,248.0f/255.0f,31.0f/255.0f,1)
		));
	//Player 5 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(82.0f/255.0f,248.0f/255.0f,31.0f/255.0f,1),
		AglVector4(31.0f/255.0f,248.0f/255.0f,143.0f/255.0f,1)
		));
	//Player 6 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(248.0f/255.0f,31.0f/255.0f,245.0f/255.0f,1),
		AglVector4(248.0f/255.0f,31.0f/255.0f,149.0f/255.0f,1)
		));
	//Player 7 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(47.0f/255.0f,77.0f/255.0f,82.0f/255.0f,1),
		AglVector4(248.0f/255.0f,220.0f/255.0f,31.0f/255.0f,1)
		));
	//Player 8 color code
	m_gradientColors.push_back(GradientMapping(
		AglVector4(232.0f/255.0f,44.0f/255.0f,12.0f/255.0f,1),
		AglVector4(248.0f/255.0f,220.0f/255.0f,31.0f/255.0f,1)
		));
	
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

AssemblageHelper::E_FileStatus EntityFactory::readAssemblageFile( string p_filePath,
															string* out_recipeName/*=NULL*/)
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

			// Set the out parameter if it has been desired.
			if (out_recipeName)
				(*out_recipeName) = newRecipe->getName();
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
	else if (type == EntityType::AnomalyModule)
	{
		if (m_client)
			e = createAnomalyModuleClient(p_packet);
		else
			e = createAnomalyModuleServer(p_packet);
	}
	else if (type == EntityType::TeslaCoilModule)
	{
		if (m_client)
			e = createTeslaCoilModuleClient(p_packet);
		else
			e = createTeslaCoilModuleServer(p_packet);
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
	else if (type == EntityType::AnomalyBomb)
	{
		if (m_client)
			e = createAnomalyBombClient(p_packet);
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
	SoundComponent* soundComponent = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "ClientShip", "Assemblages/ClientShip.asd");

	// Add network dependent components
	Transform* transform = new Transform( p_packet.translation, p_packet.rotation, 
		p_packet.scale);
	entity->addComponent( ComponentType::Transform, transform );

	entity->addComponent(ComponentType::Gradient, new GradientComponent(
		m_gradientColors[p_packet.playerID].layerOne,
		m_gradientColors[p_packet.playerID].layerTwo) );

	entity->addComponent( ComponentType::ThrustComponent, new ThrustComponent());

	entity->addComponent( new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		EntityType::Ship));

	entity->addComponent( ComponentType::TAG_Ship, new Ship_TAG());

	soundComponent = new SoundComponent();
	entity->addComponent(soundComponent);
	
	Component* component = NULL;

	/************************************************************************/
	/* Check if the owner is the same as this client.						*/
	/************************************************************************/
	if(m_client->getId() == p_packet.owner)
	{
		entity->addComponent( new ShipFlyController(3.0f, 100.0f) );
		entity->addComponent( new ShipEditController() );
		entity->addTag( ComponentType::TAG_ShipFlyMode, new ShipFlyMode_TAG );
		entity->addComponent( ComponentType::TAG_MyShip, new MyShip_TAG() );
		
		//!!!!!!!! Don't change the name of the sounds !!!!!!!!
		AudioHeader* engineSound = new AudioHeader(AudioHeader::AMBIENT, "ShipEngineIdle");
		engineSound->file = "space_ship_engine_idle.wav";
		engineSound->path = TESTSOUNDEFFECTPATH;
		engineSound->maxFrequencyOffeset = 2.0f;
		engineSound->playInterval	= AudioHeader::FOREVER;
		engineSound->sourceChannels = 1;
		engineSound->queuedPlayingState = AudioHeader::PLAY;
		engineSound->volume = 0.5f;
		soundComponent->addAudioHeader(engineSound);

		engineSound = new AudioHeader(AudioHeader::AMBIENT, "ShipEngineActive");
		engineSound->file = "space_ship_engine_active.wav";
		engineSound->path = TESTSOUNDEFFECTPATH;
		engineSound->maxFrequencyOffeset = 2.0f;
		engineSound->playInterval	= AudioHeader::FOREVER;
		engineSound->sourceChannels = 1;
		engineSound->volume = 0.5f;
		soundComponent->addAudioHeader(engineSound);

		engineSound = new AudioHeader(AudioHeader::POSITIONALSOUND, "ShipEngineIdle");
		engineSound->file = "space_ship_engine_idle.wav";
		engineSound->path = TESTSOUNDEFFECTPATH;
		engineSound->playInterval	= AudioHeader::FOREVER;
		engineSound->queuedPlayingState = AudioHeader::PLAY;
		engineSound->maxRange = 200.0f;
		soundComponent->addAudioHeader(engineSound);
		// RM-RT 2013-03-04
		/*
		entity->addComponent(new SoundComponent( TESTSOUNDEFFECTPATH,
			"Spaceship_Engine_Idle_-_Spacecraft_hovering.wav") );
		*/
		entity->addComponent( new AudioListener(1.0f) ); // This is "moved" from the camera to the ship.
		m_world->addEntity(entity);

		/************************************************************************/
		/* Attach a camera if it's the client's ship!							*/
		/************************************************************************/
		Entity* entity = m_world->getEntityManager()->getFirstEntityByComponentType(
			ComponentType::TAG_MainCamera);
		if(entity->getComponent(ComponentType::AudioListener))
		{
			entity->removeComponent(ComponentType::AudioListener); // This is "moved" from the camera to the ship.
		}
		entity->addComponent( new PlayerCameraController(90.0f) );
		entity->addComponent( new NetworkSynced(p_packet.miscData, p_packet.owner,
			EntityType::PlayerCamera) );
		entity->addComponent( new PlayerState );
		//Add a picking ray to the camera so that edit mode can be performed
		entity->addComponent( new PickComponent() );
		// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

		entity->applyComponentChanges();
	}
	else
	{
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
	AssemblageHelper::E_FileStatus status = readAssemblageFile( "Assemblages/Modules/Minigun/ServerMinigun.asd" );
	Entity* entity = entityFromRecipe( "ServerMinigun" );

	//ParticleSystemServerComponent* psServerComp = new ParticleSystemServerComponent();
	//psServerComp->addParticleSystem( ParticleSystemData( "minigun" ) );
	//entity->addComponent( psServerComp );

	entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::MinigunModule));

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createSpeedBoosterClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	// read basic assemblage
	entity = entityFromRecipeOrFile( "SpeedBooster", "Assemblages/Modules/SpeedBooster/ClientSpeedBooster.asd");

	// Add network dependent components
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, 
		(EntityType::EntityEnums)p_packet.entityType));
	// entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createSpeedBoosterServer(EntityCreationPacket p_packet)
{
	AssemblageHelper::E_FileStatus status = readAssemblageFile( "Assemblages/Modules/SpeedBooster/ServerSpeedBooster.asd" );
	Entity* entity = entityFromRecipe( "SpeedBooster" );

	entity->addComponent(ComponentType::SpeedBoosterModule, new SpeedBoosterModule());
	entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(entity->getIndex(), -1, EntityType::BoosterModule));

	m_world->addEntity(entity);
	return entity;
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

	// RM-RT 2013-03-04
	/*
	entity->addComponent( ComponentType::SoundComponent, new SoundComponent(
		TESTSOUNDEFFECTPATH, "Missile_Flight.wav" ));
	*/
	m_world->addEntity(entity);
	//static_cast<AudioBackendSystem*>(m_world->getSystem(SystemType::AudioBackendSystem))->
	//	playPositionalSoundEffect(TESTSOUNDEFFECTPATH, "Missile_Start.wav",
	//	p_packet.translation);
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
	Transform* trans = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, trans );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));

	entity->addComponent(ComponentType::ShineSpawn, new ShineSpawn(m_world->getElapsedTime(), 3.5f));

	// entity->addComponent(ComponentType::InterpolationComponent,new InterpolationComponent());
	
	// RM-RT 2013-03-04
	/*
	entity->addComponent( ComponentType::SoundComponent, new SoundComponent(
		TESTSOUNDEFFECTPATH, "Mine_Blip.wav") );
	*/

	SoundComponent* soundComponent = new SoundComponent();
	entity->addComponent(soundComponent);

	/*string name = "Unload";
	AudioHeader* unload = new AudioHeader(AudioHeader::AMBIENT, name);
	unload->file = "Mine_Unload.wav";
	unload->path = TESTSOUNDEFFECTPATH;
	unload->maxFrequencyOffeset = 2.0f;
	unload->playInterval	= (AudioHeader::PlayInterval)AudioHeader::ONCE;
	unload->sourceChannels = 1;
	unload->queuedPlayingState = AudioHeader::PLAY;
	unload->volume = 0.5f;
	soundComponent->addAudioHeader(unload);

	AudioHeader* blip = new AudioHeader(AudioHeader::POSITIONALSOUND, "Blip");
	blip->file = "Mine_Blip_v2.wav";
	blip->path = TESTSOUNDEFFECTPATH;
	blip->maxFrequencyOffeset = 2.0f;
	blip->playInterval	= (AudioHeader::PlayInterval)AudioHeader::TIMERBASED;
	blip->timerInterval = 1.0f;
	blip->sourceChannels = 1;
	blip->queuedPlayingState = AudioHeader::PLAY;
	blip->volume = 1.0f;
	blip->minRange = 0.0f;
	blip->maxRange = 100.0f;
	soundComponent->addAudioHeader(blip);*/


	Vibration* v = new Vibration(100.0f,10.0f,40.0f);
	v->enabled = false;
	entity->addComponent( ComponentType::Vibration, v );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createMineServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}

Entity* EntityFactory::createAnomalyPieces(int p_parentIndex)
{
	float effectRadius = 30.0f;
	Entity* parent = m_world->getEntity(p_parentIndex);
	if(parent != NULL)
	{
		AnomalyBomb* bomb = static_cast<AnomalyBomb*>(parent->getComponent(
			ComponentType::AnomalyBomb));
		if(bomb != NULL)
		{
			effectRadius = bomb->radius * 0.75f;
		}
	}
	for(unsigned int i=0; i<500; i++)
	{
		Entity* pieceEntity = m_world->createEntity();
		Transform* pieceTransform = new Transform();
		pieceEntity->addComponent(pieceTransform);
		pieceEntity->addComponent(new LoadMesh("shield_plate.agl"));
		pieceEntity->addComponent(new AnomalyBombEffectPiece(5.0f, effectRadius, 2.0f,
			RandomUtil::randomSingle()));
		pieceEntity->addComponent(new EntityParent(p_parentIndex,
			pieceTransform->getMatrix()));
		m_world->addEntity(pieceEntity);
	}

	return NULL;
}

void EntityFactory::createBackgroundScene()
{
	Entity* testEntity = entityFromRecipeOrFile( "test",
		"Assemblages/BackgroundScene/test.asd" );
	m_world->addEntity(testEntity);
}

Entity* EntityFactory::createShieldClient(EntityCreationPacket p_packet)
{
	Entity* shieldEntity = entityFromRecipeOrFile( "Shield",
		"Assemblages/Modules/Shield/ClientShield.asd");
	shieldEntity->setName("shieldModuleClient");
	// set transform from packet directly
	auto transform = static_cast<Transform*>(
		shieldEntity->getComponent(ComponentType::Transform));
	transform->setTranslation(p_packet.translation);
	transform->setRotation(p_packet.rotation);	
	transform->setScale(p_packet.scale);	
	// Add network dependent components
	shieldEntity->addComponent(new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		(EntityType::EntityEnums)p_packet.entityType));
	auto shipModule = static_cast<ShipModule*>(
		shieldEntity->getComponent(ComponentType::ShipModule));
	shieldEntity->addComponent(new ShieldModule());
	m_world->addEntity(shieldEntity);


	//RUINED BY ASSEMBLAGE. ASK ANTON WHY? ASK JOHAN HOW HE WANTS TO ADJUST IT!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//RUINED BY ASSEMBLAGE. ASK ANTON WHY? ASK JOHAN HOW HE WANTS TO ADJUST IT!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//RUINED BY ASSEMBLAGE. ASK ANTON WHY? ASK JOHAN HOW HE WANTS TO ADJUST IT!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//RUINED BY ASSEMBLAGE. ASK ANTON WHY? ASK JOHAN HOW HE WANTS TO ADJUST IT!!!!!!!!!!!!!!!!!!!!!!!!!!!

	return shieldEntity;
}
Entity* EntityFactory::createShieldServer(EntityCreationPacket p_packet)
{
	Entity* entity = entityFromRecipeOrFile( "Shield",
		"Assemblages/Modules/Shield/ServerShield.asd");
	auto transform = static_cast<Transform*>(entity->getComponent(ComponentType::Transform));
	transform->setTranslation(p_packet.translation);
	transform->setRotation(p_packet.rotation);
	transform->setScale(p_packet.scale);
	auto bodyInitData = static_cast<BodyInitData*>(entity->getComponent(ComponentType::BodyInitData));
	bodyInitData->m_position = p_packet.translation;
	bodyInitData->m_orientation = p_packet.rotation;
	bodyInitData->m_scale = p_packet.scale;
	// 
	entity->addComponent(ComponentType::ShieldModule, new ShieldModule());
	entity->addComponent(ComponentType::NetworkSynced, new NetworkSynced(
		entity->getIndex(), -1, EntityType::ShieldModule));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createTeslaCoilModuleClient(EntityCreationPacket p_packet)
{
	Entity* entity = entityFromRecipeOrFile( "ClientTeslaCoil",
		"Assemblages/Modules/TeslaCoil/ClientTeslaCoil.asd" );
	entity->addComponent(new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		(EntityType::EntityEnums)p_packet.entityType));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createTeslaCoilModuleServer(EntityCreationPacket p_packet)
{
	Entity* entity = entityFromRecipeOrFile( "ServerTeslaCoil",
		"Assemblages/Modules/TeslaCoil/ServerTeslaCoil.asd" );
	entity->addComponent(new NetworkSynced(entity->getIndex(), -1,
		EntityType::TeslaCoilModule));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createTeslaEffectPieceClient(AglVector3 p_forwardScale,
		float p_thicknessFactor, AglQuaternion p_rotation, AglVector3 p_sourcePosition,
		string p_meshName)
{
	Entity* entity = entityFromRecipeOrFile( "ClientTeslaEffectPiece",
		"Assemblages/Modules/TeslaCoil/ClientTeslaEffectPiece.asd" );
	// Change some components that isn't fully initialized in its assemblage:

	entity->addComponent(new LoadMesh(p_meshName));

	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createAnomalyModuleClient(EntityCreationPacket p_packet)
{
	Entity* entity = entityFromRecipeOrFile( "ClientAnomalyAccelerator",
		"Assemblages/Modules/AnomalyAccelerator/ClientAnomalyAccelerator.asd" );
	entity->addComponent(new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		(EntityType::EntityEnums)p_packet.entityType));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createAnomalyModuleServer(EntityCreationPacket p_packet)
{
	Entity* entity = entityFromRecipeOrFile( "ServerAnomalyAccelerator",
		"Assemblages/Modules/AnomalyAccelerator/ServerAnomalyAccelerator.asd" );
	entity->addComponent(new NetworkSynced(entity->getIndex(), -1,
		EntityType::AnomalyModule));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createAnomalyBombClient( EntityCreationPacket p_packet )
{
	Entity* entity = entityFromRecipeOrFile( "ClientAnomalyBomb",
		"Assemblages/Modules/AnomalyAccelerator/ClientAnomalyBomb.asd" );
	entity->addComponent(new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		(EntityType::EntityEnums)p_packet.entityType));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createAnomalyBombServer( Transform* p_transform,
	AglVector3 p_moduleVelocity, ShipModule* p_module )
{
	Entity* entity = entityFromRecipeOrFile( "ServerAnomalyBomb", 
		"Assemblages/Modules/AnomalyAccelerator/ServerAnomalyBomb.asd" );
	entity->setName("AnomalyBomb");
	// The PhysicsInitData must be created outside because it is
	// dependent of "parent" velocity and position.
	AglVector3 scale = AglVector3(1.0f, 1.0f, 1.0f);
	Transform* transform = new Transform(p_transform->getTranslation(),
		p_transform->getRotation(), scale);
	entity->addComponent(transform);
	entity->addComponent(new BodyInitData(p_transform->getTranslation(),
		p_transform->getRotation(), scale, p_moduleVelocity,
		AglVector3::zero(), 0, BodyInitData::DYNAMIC, BodyInitData::SINGLE,
		false, true));
	entity->addComponent(new NetworkSynced(entity->getIndex(), -1,
		EntityType::AnomalyBomb));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createOtherClient(EntityCreationPacket p_packet)
{
	Entity* entity = NULL;

	if (p_packet.miscData != STATICPROPTYPE_UNSPECIFIED) // Means this is either a level chamber or level gate for now.
	{
		auto levelInfoLoader = static_cast<LevelInfoLoader*>(
			m_world->getSystem(SystemType::LevelInfoLoader));
		auto fileData = levelInfoLoader->getFileData( p_packet.meshInfo, (StaticPropType)p_packet.miscData );
		if (fileData)
		{
			string asdName = fileData->assemblageName;
			entity = entityFromRecipe( asdName );
		}
		LevelPieceRoot* root = static_cast<LevelPieceRoot*>(entity->getComponent(ComponentType::LevelPieceRoot));
		if (root)
		{
			root->pieceId = p_packet.networkIdentity;
			if (p_packet.miscData == STATICPROPTYPE_LEVELPIECE)
				root->pieceRootType = PIECEROOTTYE_CHAMBER;
			else
				root->pieceRootType = PIECEROOTTYE_GATE;

			int miscSize = p_packet.additionalMisc.size();
			if (miscSize > 0)
			{
				root->connectedRootPieces.resize(miscSize);
				for (int misc = 0; misc < miscSize; misc++)
					root->connectedRootPieces[misc] = p_packet.additionalMisc[misc];
			}
		}
	}
	else	
	{
		if (p_packet.meshInfo == 1)
			entity = entityFromRecipeOrFile( "DebugSphere", "Assemblages/DebugSphere.asd" );
		else
			entity = entityFromRecipeOrFile( "DebugCube", "Assemblages/DebugCube.asd" );

	}

	// Try fetch a transform component if it exists.
	Transform* transform = NULL;
	if (entity)
	{	
		transform = static_cast<Transform*>(entity->getComponent(ComponentType::Transform));
		if (transform)
		{
			// Update the existing transform
			transform->setTranslation(p_packet.translation);
			transform->setRotation(p_packet.rotation);
			transform->setScale(p_packet.scale);
		}
		// If this else block is ran, is means no transform as been specified in the assemblage file
		else
		{
			transform = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
			entity->addComponent( transform );
		}

		m_world->addEntity(entity);
	}

	/*LevelPieceRoot* root = static_cast<LevelPieceRoot*>(entity->getComponent(ComponentType::LevelPieceRoot));
	if (root)
	{
		root->boundingSphere.position = p_packet.bsPos;
		root->boundingSphere.radius = p_packet.bsRadius;
	}*/

	return entity;
}
Entity* EntityFactory::createOtherServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
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

void EntityFactory::createHighlightParticleEmitter( ParticleSystemsComponent* p_emitters,
	AglVector3 p_spawnPosition, AglVector3 p_spawnDirection, int p_desiredIndex )
{
	AglParticleSystem particleSystem;
	particleSystem.setSpawnPoint(p_spawnPosition);
	particleSystem.setSpawnDirection(p_spawnDirection);
	particleSystem.setParticleAge(1.0f);
	particleSystem.setSpawnSpeed(1.0f);
	particleSystem.setSpawnFrequency(1.0f);
	particleSystem.setParticlesPerSpawn(100);
	particleSystem.setSpreadType(AglParticleSystemHeader::INSPACE);
	particleSystem.setSpread(0.21f);
	particleSystem.setFadeOutStart(0.0f);
	particleSystem.setFadeInStop(0.0f);
	particleSystem.setSpawnType(AglParticleSystemHeader::ONCE);
	particleSystem.setAlignmentType(AglParticleSystemHeader::VELOCITY);
	particleSystem.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	particleSystem.setParticleSpace( AglParticleSystemHeader::AglSpace_LOCAL );
	ParticleSystemInstruction particleInstruction;
	particleInstruction.textureFileName = "red-spot.png";
	particleInstruction.particleSystem = particleSystem;
	p_emitters->addParticleSystemInstruction(particleInstruction, p_desiredIndex);
}


void EntityFactory::createExplosion(const SpawnExplosionPacket& p_packet)
{
	Entity* effect = m_world->createEntity();

	ParticleSystemsComponent* particleEmitter = new ParticleSystemsComponent();

	effect->addComponent( ComponentType::Transform, new Transform());

	//Flares spreading
	AglParticleSystem flares;
	flares.setSpawnPoint(p_packet.position);
	flares.setSpawnDirection(AglVector3(0, 1, 0));
	flares.setSpread(1.0f);
	flares.setSpawnType(AglParticleSystemHeader::ONCE);
	flares.setParticlesPerSpawn(200);
	flares.setSpawnFrequency(3.0f);
	flares.setAlignmentType(AglParticleSystemHeader::VELOCITY);
	flares.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	flares.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	flares.setSpawnSpeed(30.0f);
	flares.setParticleSize(AglVector2(3.0f, 0.5f));
	flares.setParticleAge(0.5f);
	flares.setFadeOutStart(0.0f);
	flares.setSpawnOffset(1.0f);
	if (p_packet.source == ExplosionSource::MINE)
		flares.setColor(AglVector4(1, 0, 0, 1));
	//flares.setColor(AglVector4(1, 0, 0, 1));

	ParticleSystemInstruction particleInstructionFlares;
	particleInstructionFlares.textureFileName = "yellow-spot.png";
	particleInstructionFlares.particleSystem = flares;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlares);

	//Shockwave
	/*AglParticleSystem Shockwave;
	Shockwave.setSpawnPoint(p_packet.position);
	Shockwave.setSpawnDirection(AglVector3(0, 1, 0));
	Shockwave.setSpread(1.0f);
	Shockwave.setSpawnType(AglParticleSystemHeader::ONCE);
	Shockwave.setParticlesPerSpawn(200);
	Shockwave.setSpawnFrequency(3.0f);
	Shockwave.setAlignmentType(AglParticleSystemHeader::SCREEN);
	Shockwave.setSpreadType(AglParticleSystemHeader::INPLANE);
	Shockwave.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	Shockwave.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	Shockwave.setSpawnSpeed(30.0f);
	Shockwave.setParticleSize(AglVector2(3.0f, 0.5f));
	Shockwave.setParticleAge(2.0f);
	Shockwave.setFadeOutStart(0.0f);
	Shockwave.setSpawnOffset(1.0f);
	Shockwave.setMaxOpacity(0.25f);

	ParticleSystemInstruction particleInstructionShockwave;
	particleInstructionShockwave.textureFileName = "yellow-spot.png";
	particleInstructionShockwave.particleSystem = Shockwave;
	particleEmitter->addParticleSystemInstruction(particleInstructionShockwave);*/

	//Smoke
	AglParticleSystem Smoke;
	Smoke.setSpawnPoint(p_packet.position);
	Smoke.setParticleAge(1.0f);
	Smoke.setSpawnSpeed(10.0f);
	Smoke.setSpawnDirection(AglVector3(0, 1, 0));
	Smoke.setSpread(1.0f);
	Smoke.setSpawnFrequency(3.0f);

	Smoke.setFadeInStop(0.25f);
	Smoke.setFadeOutStart(0.25f);
	Smoke.setSpawnOffset(3.0f);
	Smoke.setMaxOpacity(0.25f);
	Smoke.setSpawnType(AglParticleSystemHeader::ONCE);
	Smoke.setParticlesPerSpawn(100);
	Smoke.setAlignmentType(AglParticleSystemHeader::VELOCITY);
	Smoke.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ALPHA;
	Smoke.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	Smoke.setParticleSize(AglVector2(5.0f, 5.0f));

	ParticleSystemInstruction particleInstructionSmoke;
	particleInstructionSmoke.textureFileName = "Test/smoke.png";
	particleInstructionSmoke.particleSystem = Smoke;
	particleEmitter->addParticleSystemInstruction(particleInstructionSmoke);

	//Flash
	AglParticleSystem Flash;
	Flash.setSpawnPoint(p_packet.position);
	Flash.setParticleAge(0.5f);
	Flash.setSpawnSpeed(1.0f);
	Flash.setSpawnDirection(AglVector3(0, 1, 0));
	Flash.setSpread(1.0f);
	Flash.setSpawnFrequency(3.0f);

	Flash.setFadeInStop(0.25f);
	Flash.setFadeOutStart(0.25f);
	Flash.setSpawnOffset(2.0f);
	Flash.setMaxOpacity(0.25f);
	Flash.setSpawnType(AglParticleSystemHeader::ONCE);
	Flash.setParticlesPerSpawn(5);
	Flash.setAlignmentType(AglParticleSystemHeader::OBSERVER);
	Flash.getHeaderPtr()->blendMode = AglParticleSystemHeader::AglBlendMode_ADDITIVE;
	Flash.setSpawnSpace(AglParticleSystemHeader::AglSpace_GLOBAL);
	Flash.setParticleSize(AglVector2(20.0f, 20.0f));
	if (p_packet.source == ExplosionSource::MINE)
		Flash.setColor(AglVector4(1, 0, 0, 1));
	//Flash.setColor(AglVector4(1, 0, 0, 1));

	ParticleSystemInstruction particleInstructionFlash;
	particleInstructionFlash.textureFileName = "yellow-spot.png";
	particleInstructionFlash.particleSystem = Flash;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlash);

	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);

	SoundComponent* soundComponent = new SoundComponent();
	effect->addComponent(soundComponent);

	Component* component = NULL;

	string name = "Explosion";
	AudioHeader* explodeSound = new AudioHeader(AudioHeader::POSITIONALSOUND, name);
	explodeSound->file = "bomb-03.wav";
	explodeSound->path = TESTSOUNDEFFECTPATH;
	explodeSound->maxFrequencyOffeset = 2.0f;
	explodeSound->playInterval	= (AudioHeader::PlayInterval)AudioHeader::ONCE;
	explodeSound->sourceChannels = 1;
	explodeSound->queuedPlayingState = AudioHeader::PLAY;
	explodeSound->volume = 0.5f;
	soundComponent->addAudioHeader(explodeSound);


	m_world->addEntity(effect);
}

AglVector4 EntityFactory::getPlayersFirstGradientLevel( ) const{
	return m_gradientColors[m_client->getPlayerID()].layerOne;
}

AglVector4 EntityFactory::getPlayersSecondGradientLevel( ) const{
	return m_gradientColors[m_client->getPlayerID()].layerTwo;
}
