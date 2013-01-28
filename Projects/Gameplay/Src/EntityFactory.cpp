#include "EntityFactory.h"

#include <AssemblageReader.h>
#include <Entity.h>
#include "AudioListener.h"
#include "CameraInfo.h"
#include "ConnectionPointSet.h"
#include "Extrapolate.h"
#include "GameplayTags.h"
#include "GraphicsBackendSystem.h"
#include "LightsComponent.h"
#include "LookAtEntity.h"
#include "MainCamera.h"
#include "NetworkSynced.h"
#include "PickComponent.h"
#include "PlayerCameraController.h"
#include "PlayerScore.h"
#include "RenderInfo.h"
#include "ShipEditController.h"
#include "ShipFlyController.h"
#include "Transform.h"
#include "ParticleRenderSystem.h"
#include "LightBlinker.h"

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
	}

	return meal;
}
Entity* EntityFactory::entityFromPacket(EntityCreationPacket p_packet)
{
	EntityType::EntityEnums type = static_cast<EntityType::EntityEnums>(p_packet.entityType);
	if (type == EntityType::Ship)
	{
		if (m_client)
			return createShipEntityClient(p_packet);
		else
			return createShipEntityServer(p_packet);
	}
	else if (type == EntityType::MineLayerModule)
	{
		if (m_client)
			return createMineLayerClient(p_packet);
		else
			return createMineLayerServer(p_packet);
	}
	else if (type == EntityType::RocketLauncherModule)
	{
		if (m_client)
			return createRocketLauncherClient(p_packet);
		else
			return createRocketLauncherServer(p_packet);
	}
	else if (type == EntityType::BoosterModule)
	{
		if (m_client)
			return createSpeedBoosterClient(p_packet);
		else
			return createSpeedBoosterServer(p_packet);
	}
	else if (type == EntityType::MinigunModule)
	{
		if (m_client)
			return createMinigunClient(p_packet);
		else
			return createMinigunServer(p_packet);
	}
	else if (type > EntityType::ShipModuleStart && type < EntityType::EndModule)
	{
		if (m_client)
			return createModuleClient(p_packet);
		else
			return createModuleServer(p_packet);
	}
	else if (type == EntityType::LaserSight)
	{
		if (m_client)
			return createLaserSightClient(p_packet);
		else
			return createLaserSightServer(p_packet);
	}
	else if (type == EntityType::ParticleSystem)
	{
		if (m_client)
			return createParticleSystemClient(p_packet);
		else
			return createParticleSystemServer(p_packet);
	}
	else if (type == EntityType::SelectionSphere)
	{
		if (m_client)
			return createSelectionSphereClient(p_packet);
		else
			return createSelectionSphereServer(p_packet);
	}
	else if (type == EntityType::Mine)
	{
		if (m_client)
			return createMineClient(p_packet);
		else
			return createMineServer(p_packet);
	}
	else if (type == EntityType::Rocket)
	{
		if (m_client)
			return createRocketClient(p_packet);
		else
			return createRocketServer(p_packet);
	}
	else if (type == EntityType::Shield)
	{
		if (m_client)
			return createShieldClient(p_packet);
		else
			return createShieldServer(p_packet);
	}
	else if (type == EntityType::Other)
	{
		if (m_client)
			return createOtherClient(p_packet);
		else
			return createOtherServer(p_packet);
	}
	DEBUGPRINT(("Network Warning: Received unknown entity type from server!\n"));
	return NULL;
}

Entity* EntityFactory::createShipEntityClient(EntityCreationPacket p_packet)
{
	int shipMeshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("Ship.agl");

	//shipMeshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		//SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

	Entity* entity = NULL;
	//entity = m_world->createEntity();

	readAssemblageFile( "Assemblages/ClientShip.asd" );
	entity = entityFromRecipe( "ClientShip" );

	Transform* transform = new Transform( p_packet.translation, p_packet.rotation, 
		p_packet.scale);

	Component* component = new RenderInfo( shipMeshId );
	entity->addComponent( ComponentType::RenderInfo, component );
	entity->addComponent( ComponentType::Transform, transform );
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, EntityType::Ship));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

//<<<<<<< HEAD
	//LightsComponent* lightComp = new LightsComponent();
	//Light floodLight;
	//float range = 100.0f;
	//AglMatrix::componentsToMatrix(
	//	floodLight.offsetMat,
	//	AglVector3( range, range, range*20 ),
	//	AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
	//	AglVector3( 2.0f, 0.0f, 0.0f )
	//	);
	//floodLight.instanceData.range = range*20;
	//floodLight.instanceData.attenuation[1] = 0.1f;
	//floodLight.instanceData.spotPower = 8.0f;
	//floodLight.instanceData.lightDir[0] = 0.0f;
	//floodLight.instanceData.lightDir[1] = 0.0f;
	//floodLight.instanceData.lightDir[2] = 1.0f;
	//floodLight.instanceData.diffuse[0] = 0.0f;
	//floodLight.instanceData.diffuse[1] = 1.0f;
	//floodLight.instanceData.diffuse[2] = 0.0f;
	////floodLight.instanceData.ambient[0] = 0.0f;
	////floodLight.instanceData.ambient[1] = 0.0f;
	////floodLight.instanceData.ambient[2] = 1.0f;
	////floodLight.instanceData.specular[0] = 2.0f;
	////floodLight.instanceData.specular[1] = 2.0f;
	////floodLight.instanceData.specular[2] = 2.0f;
	//floodLight.instanceData.enabled = true;
	//floodLight.instanceData.type = LightTypes::E_LightTypes_SPOT;

	//lightComp->addLight( floodLight );
	//AglMatrix::componentsToMatrix(
	//	floodLight.offsetMat,
	//	AglVector3( range, range, range*20 ),
	//	AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
	//	AglVector3( -2.0f, 0.0f, 0.0f )
	//	);
	//floodLight.instanceData.diffuse[0] = 1.0f;
	//floodLight.instanceData.diffuse[1] = 0.0f;
	//floodLight.instanceData.diffuse[2] = 0.0f;
	//lightComp->addLight( floodLight );

	//entity->addComponent( ComponentType::LightsComponent, lightComp);
//=======
//	LightsComponent* lightComp = new LightsComponent();
//	Light floodLight;
//	float range = 200.0f;
//	floodLight.instanceData.range = range;
//	floodLight.instanceData.attenuation[1] = 0.1f;
//	floodLight.instanceData.spotPower = 8.0f;
//	floodLight.instanceData.lightDir[0] = 0.0f;
//	floodLight.instanceData.lightDir[1] = 0.0f;
//	floodLight.instanceData.lightDir[2] = 1.0f;
//	floodLight.instanceData.diffuse[0] = 1.0f;
//	floodLight.instanceData.diffuse[1] = 1.0f;
//	floodLight.instanceData.diffuse[2] = 1.0f;
////	floodLight.instanceData.specular[0] = 2.0f;
////	floodLight.instanceData.specular[1] = 2.0f;
////	floodLight.instanceData.specular[2] = 2.0f;
//	floodLight.instanceData.enabled = true;
//	floodLight.instanceData.type = LightTypes::E_LightTypes_SPOT;
//	// Left spotlight
//	AglMatrix::componentsToMatrix(
//		floodLight.offsetMat,
//		AglVector3( range, range, range ),
//		AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
//		AglVector3( -2.0f, 0.0f, 5.0f )
//		);
//	lightComp->addLight( floodLight );
//	// Right spotlight
//	AglMatrix::componentsToMatrix(
//		floodLight.offsetMat,
//		AglVector3( range, range, range ),
//		AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
//		AglVector3( 2.0f, 0.0f, 5.0f )
//		);
//	lightComp->addLight( floodLight );
//
//	floodLight.instanceData.type = LightTypes::E_LightTypes_POINT;
//	range = 2.0f;
//	floodLight.instanceData.range = range;
//	// Left pointlight
//	AglMatrix::componentsToMatrix(
//		floodLight.offsetMat,
//		AglVector3( range, range, range ),
//		AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
//		AglVector3( -3.0f, 0.0f, -2.0f )
//		);
//	floodLight.instanceData.diffuse[0] = 1.0f;
//	floodLight.instanceData.diffuse[1] = 0.0f;
//	floodLight.instanceData.diffuse[2] = 0.0f;
//	lightComp->addLight( floodLight );
//	// Right pointlight
//	AglMatrix::componentsToMatrix(
//		floodLight.offsetMat,
//		AglVector3( range, range, range ),
//		AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
//		AglVector3( 3.0f, 0.0f, -2.0f )
//		);
//	floodLight.instanceData.diffuse[0] = 0.0f;
//	floodLight.instanceData.diffuse[1] = 1.0f;
//	floodLight.instanceData.diffuse[2] = 0.0f;
//	lightComp->addLight( floodLight );
//
//	entity->addComponent( ComponentType::LightsComponent, lightComp);
//>>>>>>> 6992faf7bc6868a43996e5118a42de6f75d2adfd

	/************************************************************************/
	/* Check if the owner is the same as this client.						*/
	/************************************************************************/
	if(m_client->getId() == p_packet.owner)
	{
		component = new ShipFlyController(5.0f, 50.0f);
		entity->addComponent( ComponentType::ShipFlyController, component );

		component = new ShipEditController();
		entity->addComponent( ComponentType::ShipEditController, component);

		entity->addTag(ComponentType::TAG_ShipFlyMode, new ShipFlyMode_TAG());

		ConnectionPointSet* connectionPointSet = new ConnectionPointSet();
		connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
			AglMatrix::createTranslationMatrix(AglVector3(2.5f, 0, 0))));
		connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
			AglMatrix::createTranslationMatrix(AglVector3(-2.5f, 0, 0))));
		connectionPointSet->m_connectionPoints.push_back(ConnectionPoint(
			AglMatrix::createTranslationMatrix(AglVector3(0, 2.5f, 0))));
		entity->addComponent(ComponentType::ConnectionPointSet, connectionPointSet);
		// NOTE: (Johan) Moved the audio listener to the ship instead of the camera
		// because it was really weird to hear from the camera. This can of course
		// be changed back if game play fails in this way, but it's at least more
		// convenient for debugging!
		component = new AudioListener();
		entity->addComponent(ComponentType::AudioListener, component);
	}

	/************************************************************************/
	/* HACK: Score should probably be located in another entity.			*/
	/************************************************************************/
	component = new PlayerScore();
	entity->addComponent( ComponentType::PlayerScore, component );
	m_world->addEntity(entity);

	/************************************************************************/
	/* Attach a camera if it's the clients ship!							*/
	/************************************************************************/
	if(p_packet.owner == m_client->getId())
	{
		int shipId = entity->getIndex();
		float aspectRatio = 
			static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getAspectRatio();

		entity = m_world->createEntity();
		component = new CameraInfo( aspectRatio );
		entity->addComponent( ComponentType::CameraInfo, component );
		component = new MainCamera();
		entity->addComponent( ComponentType::MainCamera, component );
		component = new Transform( -5.0f, 0.0f, -5.0f );
		entity->addComponent( ComponentType::Transform, component );
		component = new LookAtEntity(shipId,
			AglVector3(0,6,-13),
			AglQuaternion::identity(),
			10.0f,
			10.0f,
			10.0f);
		entity->addComponent( ComponentType::LookAtEntity, component );
		// default tag is follow
		entity->addTag(ComponentType::TAG_LookAtFollowMode, new LookAtFollowMode_TAG() );
		entity->addComponent(ComponentType::PlayerCameraController, new PlayerCameraController() );

		//Add a picking ray to the camera so that edit mode can be performed
		entity->addComponent(ComponentType::PickComponent, new PickComponent());

		m_world->addEntity(entity);

		/************************************************************************/
		/* This is where the audio listener is created and therefor the master  */
		/* volume is added to Ant Tweak Bar here.								*/
		/************************************************************************/
		AntTweakBarWrapper::getInstance()->addWriteVariable( 
			AntTweakBarWrapper::OVERALL,
			"Master_volume", TwType::TW_TYPE_FLOAT, 
			static_cast<AudioListener*>(component)->getMasterVolumeRef(),
			"group=Sound min=0 max=10 step=0.001 precision=3");
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
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("MineWeaponFinal.agl");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createMineLayerServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createRocketLauncherClient(EntityCreationPacket p_packet)
{
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("rocket_launcher.agl");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createRocketLauncherServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createMinigunClient(EntityCreationPacket p_packet)
{
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("minigun.agl");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createMinigunServer(EntityCreationPacket p_packet)
{
	return NULL;
}
Entity* EntityFactory::createSpeedBoosterClient(EntityCreationPacket p_packet)
{
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("SpeedBooster.agl");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createSpeedBoosterServer(EntityCreationPacket p_packet)
{
	return NULL;
}
Entity* EntityFactory::createModuleClient(EntityCreationPacket p_packet)
{
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

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
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

	if (p_packet.meshInfo == 1)
		meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("P_sphere");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

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
	AglParticleSystemHeader h;
	if (p_packet.meshInfo == 0)
	{
		h.particleSize = AglVector2(2, 2);
		h.alignmentType = AglParticleSystemHeader::OBSERVER;
		h.spawnFrequency = 10;
		h.spawnSpeed = 5.0f;
		h.spread = 0.0f;
		h.fadeOutStart = 2.0f;
		h.fadeInStop = 0.0f;
		h.particleAge = 2;
		h.maxOpacity = 1.0f;
		h.color = AglVector4(0, 1, 0, 1.0f);
	}
	else
	{
		h.particleAge = 1;
		h.spawnSpeed = 0.02;
		h.spread = 1.0f;
		h.spawnFrequency = 10;
		h.color = AglVector4(0, 1.0f, 0.7f, 1.0f);
		h.fadeInStop = 0.5f;
		h.fadeOutStart = 0.5f;
		h.spawnOffset = 4.0f;
		h.maxOpacity = 0.5f;
		h.spawnOffsetType = AglParticleSystemHeader::ONSPHERE;
		h.particleSize = AglVector2(1.0f, 1.0f);
	}

	ParticleRenderSystem* gfx = static_cast<ParticleRenderSystem*>(m_world->getSystem(
		SystemType::ParticleRenderSystem ));
	//gfx->addParticleSystem();
	gfx->addParticleSystem(h, p_packet.networkIdentity);
	return NULL;
}
Entity* EntityFactory::createParticleSystemServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createSelectionSphereClient(EntityCreationPacket p_packet)
{
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

	if (p_packet.meshInfo == 1)
		meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("P_sphere");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

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
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("rocket.agl");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createRocketServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
Entity* EntityFactory::createMineClient(EntityCreationPacket p_packet)
{
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("MineFinal.agl");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );
	
	LightsComponent* lightComp = new LightsComponent();
	Light floodLight;
	float range = 2.0f;
	AglMatrix::componentsToMatrix(
		floodLight.offsetMat,
		AglVector3( range, range, range ),
		AglQuaternion::constructFromAxisAndAngle( AglVector3( .0f, .0f, .0f), .0f ),
		AglVector3( 0.0f, 0.0f, 0.0f )
		);
	floodLight.instanceData.range = range;
	floodLight.instanceData.attenuation[1] = 0.1f;
	floodLight.instanceData.diffuse[0] = 1.0f;
	floodLight.instanceData.diffuse[1] = 0.3f;
	floodLight.instanceData.diffuse[2] = 0.3f;
	floodLight.instanceData.enabled = true;
	floodLight.instanceData.type = LightTypes::E_LightTypes_POINT;
	lightComp->addLight(floodLight);
	entity->addComponent( ComponentType::LightsComponent, lightComp );

	entity->addComponent( ComponentType::LightBlinker, new LightBlinker(5.0f) );
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
	int meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

	if (p_packet.meshInfo == 1)
		meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem ))->getMeshId("P_sphere");

	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);
	entity->addComponent(ComponentType::NetworkSynced,
		new NetworkSynced(p_packet.networkIdentity, p_packet.owner, (EntityType::EntityEnums)p_packet.entityType));
	entity->addComponent( ComponentType::Extrapolate, new Extrapolate() );

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createShieldServer(EntityCreationPacket p_packet)
{
	return NULL;
}
Entity* EntityFactory::createOtherClient(EntityCreationPacket p_packet)
{
	int meshId = -1;

	GraphicsBackendSystem* gfxBackend = static_cast<GraphicsBackendSystem*>(
		m_world->getSystem( SystemType::GraphicsBackendSystem ));

	if (p_packet.isLevelProp)
	{
		meshId = gfxBackend->getMeshId(m_levelPieceMapping.getModelFileName(p_packet.meshInfo));
	}
	else	
	{
		meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getMeshId("P_cube");

		if (p_packet.meshInfo == 1)
			meshId = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
			SystemType::GraphicsBackendSystem ))->getMeshId("P_sphere");
	}
	Entity* entity = m_world->createEntity();
	Component* component = new Transform(p_packet.translation, p_packet.rotation, p_packet.scale);
	entity->addComponent( ComponentType::Transform, component );
	component = new RenderInfo(meshId);
	entity->addComponent(ComponentType::RenderInfo, component);

	m_world->addEntity(entity);
	return entity;
}
Entity* EntityFactory::createOtherServer(EntityCreationPacket p_packet)
{
	//Not moved here yet!
	return NULL;
}
