#include "MenuSystem.h"
#include "LibRocketBackendSystem.h"
#include "LibRocketEventManagerSystem.h"
#include "ClientConnectToServerSystem.h"
#include <Globals.h>
#include <ToString.h>
#include "MenuItem.h"
#include "ClientStateSystem.h"
#include "LobbySystem.h"
#include "SettingsSystem.h"
#include "MenuBackgroundSceneSystem.h"
#include <TcpClient.h>

MenuSystem::MenuSystem(TcpClient* p_tcpClient)
	: EntitySystem( SystemType::MenuSystem)
{
	m_joinIdx		= -1;
	m_hostIdx		= -1;
	m_lobbyDocIdx	= -1;
	m_disconnectPopupIdx = -1;
	m_loadingWindowIdx = -1;
	m_tcpClient		= p_tcpClient;
}


MenuSystem::~MenuSystem()
{
}

void MenuSystem::initialize()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	auto rocketEventManager = static_cast<LibRocketEventManagerSystem*>(
		m_world->getSystem(SystemType::LibRocketEventManagerSystem));

	auto connectToServerSys = static_cast<ClientConnectToServerSystem*>(
		m_world->getSystem(SystemType::ClientConnectoToServerSystem));

	rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"main_menu");
	rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"play");
	m_joinIdx = rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"join");
	m_hostIdx = rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"host");
	rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"options");
	rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"credits");
	m_lobbyDocIdx = rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"lobby");
	m_disconnectPopupIdx = rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),
		"connection_lost");
	m_loadingWindowIdx = rocketBackend->loadDocument(GUI_MENU_PATH.c_str(),"loading");

	rocketEventManager->loadWindow("main_menu");
}

void MenuSystem::process()
{	
	ClientStateSystem* gameState = static_cast<ClientStateSystem*>(
		m_world->getSystem(SystemType::ClientStateSystem));

	if(gameState->getStateDelta(GameStates::LOADING) == EnumGameDelta::ENTEREDTHISFRAME){
		auto rocketEventManager = static_cast<LibRocketEventManagerSystem*>(
			m_world->getSystem(SystemType::LibRocketEventManagerSystem));
		rocketEventManager->clearDocumentStack();
		rocketEventManager->loadWindow("loading");
	}
	else if(gameState->getStateDelta(GameStates::LOBBY) == EnumGameDelta::ENTEREDTHISFRAME){
		auto rocketEventManager = static_cast<LibRocketEventManagerSystem*>(
			m_world->getSystem(SystemType::LibRocketEventManagerSystem));

		static_cast<LobbySystem*>(
			m_world->getSystem(SystemType::LobbySystem))->resetAllPlayers();
		
		auto rocketBackend = static_cast<LibRocketBackendSystem*>(
			m_world->getSystem(SystemType::LibRocketBackendSystem));
		rocketBackend->updateElement(m_lobbyDocIdx, "player_ready", "Ready");

		string temp = "Name: ";
		temp += m_tcpClient->getServerName();
		rocketBackend->updateElement(m_lobbyDocIdx, "server_name", temp.c_str());
		temp = "Round Time(sec): ";
		temp += toString(m_tcpClient->getServerGameTime());
		rocketBackend->updateElement(m_lobbyDocIdx, "server_time", temp.c_str());

		rocketEventManager->clearDocumentStack();
		rocketEventManager->loadWindow("lobby");
	}
	else if(gameState->getStateDelta(GameStates::MENU) == EnumGameDelta::ENTEREDTHISFRAME){
		auto rocketEventManager = static_cast<LibRocketEventManagerSystem*>(
			m_world->getSystem(SystemType::LibRocketEventManagerSystem));

		rocketEventManager->clearDocumentStack();
		rocketEventManager->loadWindow("main_menu");

		if (gameState->getStateDelta(GameStates::INGAME) == EnumGameDelta::EXITTHISFRAME){
			auto menuBackgroundSys = static_cast<MenuBackgroundSceneSystem*>(
				m_world->getSystem(SystemType::MenuBackgroundSceneSystem));
			menuBackgroundSys->setEnabled(true);
		}
	}

}

void MenuSystem::endLoadingState()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	rocketBackend->hideDocument(m_loadingWindowIdx);

}

void MenuSystem::displayDisconnectPopup()
{
	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	// Must check that it isn't visible first!
	if (!rocketBackend->isDocumentVisible(m_disconnectPopupIdx))
	{
		auto rocketEventManager = static_cast<LibRocketEventManagerSystem*>(
			m_world->getSystem(SystemType::LibRocketEventManagerSystem));

		rocketEventManager->loadWindow("connection_lost", Rocket::Core::ElementDocument::FOCUS);
	}
}

void MenuSystem::setJoin()
{
	auto gameOptions = static_cast<SettingsSystem*>
		(m_world->getSystem(SystemType::SettingsSystem));

	GameSettingsInfo settings = gameOptions->getSettings();

	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	rocketBackend->changeValue(m_joinIdx, "1", settings.ip);
	rocketBackend->changeValue(m_joinIdx, "2", settings.port);
	rocketBackend->changeValue(m_joinIdx, "3", settings.playerName);
}

void MenuSystem::setHost()
{
	auto gameOptions = static_cast<SettingsSystem*>
		(m_world->getSystem(SystemType::SettingsSystem));

	GameSettingsInfo settings = gameOptions->getSettings();

	auto rocketBackend = static_cast<LibRocketBackendSystem*>(
		m_world->getSystem(SystemType::LibRocketBackendSystem));

	rocketBackend->changeValue(m_hostIdx, "4", settings.serverName);
	rocketBackend->changeValue(m_hostIdx, "5", settings.port);
	rocketBackend->changeValue(m_hostIdx, "6", settings.playerName);
	rocketBackend->changeValue(m_hostIdx, "7", toString(settings.defaultGameTime));
}
