#include "PreloaderSystem.h"
#include "EntityFactory.h"
#include <Globals.h>
#include "GraphicsBackendSystem.h"
#include <GraphicsWrapper.h>

PreloaderSystem::PreloaderSystem()
	: EntitySystem(SystemType::PreloaderSystem)
{
	m_entityFactory = NULL;
	m_gfxBackend = NULL;
}

PreloaderSystem::~PreloaderSystem()
{
}

void PreloaderSystem::initialize()
{
	m_entityFactory = static_cast<EntityFactory*>(m_world->getSystem(
		SystemType::EntityFactory));
	initAssemblages();

	m_gfxBackend = static_cast<GraphicsBackendSystem*>(m_world->getSystem(
		SystemType::GraphicsBackendSystem));
	initMeshes();
}

void PreloaderSystem::initAssemblages()
{
	moduleAssemblages();
	levelAssemblages();
	unsortedAssemblages();
}

void PreloaderSystem::moduleAssemblages()
{
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/AnomalyAccelerator/ClientAnomalyAccelerator.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/AnomalyAccelerator/ClientAnomalyBomb.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/MineLayer/ClientMineLayer.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/Minigun/ClientMinigun.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/RocketLauncher/ClientRocketLauncher.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/Shield/ClientShield.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/SpeedBooster/ClientSpeedBooster.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/TeslaCoil/ClientTeslaCoil.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Modules/TeslaCoil/ClientTeslaEffectPiece.asd");
}

void PreloaderSystem::levelAssemblages()
{
	// Chambers
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/alienChamberClient.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/ancientChamberClient.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/mineChamberClient.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/prisonChamberClient.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/sandChamberClient.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/smallchamber1Client.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/smallchamber2Client.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/smallchamber3Client.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/smallchamber4Client.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/vegetationChamberClient.asd");

	// Plugs
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/plugPieceClosedClient.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/plugPieceOpenedClient.asd");

	// LevelGen
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Level Pieces/LevelGenClient.asd");
}

void PreloaderSystem::unsortedAssemblages()
{
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "blueLight.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "ClientShip.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Mine.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "Rocket.asd");
	m_entityFactory->readAssemblageFile(ASSEMBLAGESPATH + "ServerShip.asd");
}

void PreloaderSystem::initMeshes()
{
	loadModuleMeshes();
}

void PreloaderSystem::loadModuleMeshes()
{
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("anomaly_launcher.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("anomaly_bomb.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("MineWeaponFinal.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("MinigunAnim.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("rocket_launcher.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("shield_module.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("shield_plate.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("speed_booster.agl", &MODELPATH, false);
	m_gfxBackend->getGfxWrapper()->createModelsFromFile("TeslaCoil.agl", &MODELPATH, false);
}