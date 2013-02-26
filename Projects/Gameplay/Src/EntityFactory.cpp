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
			e = createAnomalyClient(p_packet);
		else
			e = createAnomalyServer(p_packet);
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

	entity->addComponent(ComponentType::Gradient, new GradientComponent(
		m_gradientColors[p_packet.playerID].layerOne,
		m_gradientColors[p_packet.playerID].layerTwo) );

	//m_playerCounter++;

	entity->addComponent( new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		EntityType::Ship));
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
		
		ParticleSystemsComponent* emitters = static_cast<ParticleSystemsComponent*>(
			entity->getComponent( ComponentType::ParticleSystemsComponent ) );

		if( emitters == NULL ) {
			emitters = new ParticleSystemsComponent();
			entity->addComponent( emitters );
		}
		createHighlightParticleEmitter(emitters, AglVector3(0.0f, -2.0f, -5.0f), // Down
			AglVector3(0.0f, 0.0f, -1.0f), 0);
		createHighlightParticleEmitter(emitters, AglVector3(0.0f, -7.0f, 2.0f), // Forward
			AglVector3(0.0f, 1.0f, 1.0f), 1);
		createHighlightParticleEmitter(emitters, AglVector3(-4.5f, -2.0f, 2.5f), // Left
			AglVector3(-1.0f, 0.0f, 0.0f), 2);
		createHighlightParticleEmitter(emitters, AglVector3(4.5f, -2.0f, 2.5f), // Right
			AglVector3(1.0f, 0.0f, 0.0f), 3);

		entity->addComponent(new PositionalSoundSource( TESTSOUNDEFFECTPATH,
			"Spaceship_Engine_Idle_-_Spacecraft_hovering.wav") );

		entity->addComponent( new AudioListener(1.0f) ); // This is "moved" from the camera to the ship.
	}
	entity->addComponent( new PlayerScore() );
	entity->addComponent( ComponentType::TAG_Ship, new Ship_TAG());

	m_world->addEntity(entity);

	/************************************************************************/
	/* Attach a camera if it's the client's ship!							*/
	/************************************************************************/
	if(p_packet.owner == m_client->getId())
	{
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
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
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
	shieldEntity->setName("shieldModuleClient");
	// Add network dependent components
	Transform* transform = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	shieldEntity->addComponent(transform );
	shieldEntity->addComponent(new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		(EntityType::EntityEnums)p_packet.entityType));
	shieldEntity->addComponent(new LoadMesh("shield_module.agl"));
	ShipModule* shipModule = new ShipModule();
	shieldEntity->addComponent(shipModule);
	shieldEntity->addComponent(new ShieldModule());
	m_world->addEntity(shieldEntity);

	const int plateCount = 120;
	vector<Entity*> plateEntities;
	plateEntities.resize(120);
	for(unsigned int i=0; i<plateCount; i++)
	{
		Entity* entity = m_world->createEntity();
		entity->setName("shieldPlateClient");
		plateEntities[i] = entity;
		float spawnX, spawnY;
		circularRandom(&spawnX, &spawnY, true);
		AglVector3 spawnPoint = AglVector3(0, 6.5f, 0); // Replace with real spawn point.
		float radius = 10.0f;
		AglVector3 position = spawnPoint + AglVector3(radius * spawnX, 0, radius * spawnY);
		position.normalize();
		AglQuaternion plateRotation = AglQuaternion::rotateToFrom(AglVector3(0, 1.0f, 0.0f), position);
		position = position * spawnPoint.length();
		float plateScale = 1.0f;
		Transform* plateTransform = new Transform(position, plateRotation,
			AglVector3(plateScale, plateScale, plateScale));
		entity->addComponent(plateTransform);
		entity->addComponent(new LoadMesh("shield_plate.agl"));
		entity->addComponent(new EntityParent(shieldEntity->getIndex(),
			plateTransform->getMatrix()));
		entity->addComponent(new ShieldPlate(0.2f + 0.8f * (float)rand()/(float)RAND_MAX));
		entity->setEnabled(false);
		m_world->addEntity(entity);
	}
	shipModule->addActivationEvent(new ShieldModuleActivationClient(plateEntities,
		shieldEntity, static_cast<AudioBackendSystem*>(
		m_world->getSystem(SystemType::AudioBackendSystem))));

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

Entity* EntityFactory::createAnomalyClient(EntityCreationPacket p_packet)
{
	AssemblageHelper::E_FileStatus status = readAssemblageFile(
		"Assemblages/Modules/AnomalyAccelerator/ClientAnomalyAccelerator.asd" );
	Entity* entity = entityFromRecipe( "ClientAnomalyAccelerator" );
	entity->addComponent(new NetworkSynced(p_packet.networkIdentity, p_packet.owner,
		(EntityType::EntityEnums)p_packet.entityType));
	m_world->addEntity(entity);
	return entity;
}

Entity* EntityFactory::createAnomalyServer(EntityCreationPacket p_packet)
{
	AssemblageHelper::E_FileStatus status = readAssemblageFile(
		"Assemblages/Modules/AnomalyAccelerator/ServerAnomalyAccelerator.asd" );
	Entity* entity = entityFromRecipe( "ServerAnomalyAccelerator" );
	entity->addComponent(new NetworkSynced(entity->getIndex(), -1,
		EntityType::AnomalyModule));
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
		// entity = entityFromRecipeOrFile( "DebugSphere", "Assemblages/DebugSphere.asd" );
		auto levelInfoLoader = static_cast<LevelInfoLoader*>(
			m_world->getSystem(SystemType::LevelInfoLoader));
		auto fileData = levelInfoLoader->getFileData( p_packet.meshInfo );
		string asdName = (levelInfoLoader->getFileData( p_packet.meshInfo ))->assemblageName;
		entity = entityFromRecipe( asdName );
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
	return entity;
}
Entity* EntityFactory::createOtherServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}

void EntityFactory::circularRandom( float* p_spawnX, float* p_spawnY,
	bool p_warpCompensation )
{
	if(!p_warpCompensation)
	{
		float x = 0.0f;
		float y = 0.0f;
		do
		{
			x = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
			y = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
		} while( x * x + y * y > 1.0f );
		*p_spawnX = x;
		*p_spawnY = y;
	}
	else
	{
		// NOTE: This version of circular random compensates for sphere warping
		// quite roughly. With not too high of a warp angle it looks nice.
		// It is much slower than the simple circular random generation.
		float x = 0.0f;
		float y = 0.0f;
		do
		{
			float x_rand = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
			float y_rand = 2.0f * ((float)rand()/(float)RAND_MAX - 0.5f);
			x = x_rand;
			y = y_rand;
		} while( x * x + y * y > 1.0f );
		float len = sqrt(x * x + y * y);
		x = x * len;
		y = y * len;
		*p_spawnX = x;
		*p_spawnY = y;
	}
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
	//Flash.setColor(AglVector4(1, 0, 0, 1));

	ParticleSystemInstruction particleInstructionFlash;
	particleInstructionFlash.textureFileName = "yellow-spot.png";
	particleInstructionFlash.particleSystem = Flash;
	particleEmitter->addParticleSystemInstruction(particleInstructionFlash);

	effect->addComponent( ComponentType::ParticleSystemsComponent, particleEmitter);
	m_world->addEntity(effect);
}
