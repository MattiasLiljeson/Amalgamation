#include "ClientConnectToServerSystem.h"
#include "Control.h"
#include <TcpClient.h>
#include "InputBackendSystem.h"
#include "LevelHandlerSystem.h"
#include "GameState.h"
#include "ClientStateSystem.h"

ClientConnectToServerSystem::ClientConnectToServerSystem(TcpClient* p_tcpClient,
														 bool p_connectDirectly/* =false */)
	: EntitySystem( SystemType::ClientConnectoToServerSystem )
{
	m_tcpClient = p_tcpClient;
	m_connectStraightAway = p_connectDirectly;
	
	m_isLookingForConnection = false;

	m_serverAddress = "127.0.0.1";
	m_serverPort = "1337";
}

ClientConnectToServerSystem::~ClientConnectToServerSystem()
{
}

void ClientConnectToServerSystem::initialize()
{
	if(m_connectStraightAway){
		connectToNetworkAddress();
	}
}

void ClientConnectToServerSystem::process()
{
	if( m_tcpClient->hasActiveConnection() )
	{
		m_isLookingForConnection = false;
		m_world->getSystem(SystemType::ClientPacketHandlerSystem)->setEnabled(true);
		setEnabled(false);

		// Clears and hides all currently visible documents.
		//eventManagerSys->clearDocumentStack();
		
		auto levelHandler = static_cast<LevelHandlerSystem*>(
			m_world->getSystem(SystemType::LevelHandlerSystem));
		levelHandler->destroyLevel();
	}

	InputBackendSystem* inputBackend = static_cast<InputBackendSystem*>
		(m_world->getSystem(SystemType::InputBackendSystem));
	ClientStateSystem* stateSystem = static_cast<ClientStateSystem*>(m_world->
		getSystem(SystemType::ClientStateSystem));
	if( stateSystem->getStateDelta(GameStates::LOBBY) == EnumGameDelta::ENTEREDTHISFRAME){
		connectToNetworkAddress();
		// Disable the menu background scene.
		//m_world->getSystem(SystemType::MenuBackgroundSceneSystem)->setEnabled(false);
	}

//	if(inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys_F4) > 0 ){
//		connectToNetworkAddress();
//		state->setStatesDelta(GameStates::INGAME, EnumGameDelta::ENTEREDTHISFRAME);
//		// Disable the menu background scene.
//		m_world->getSystem(SystemType::MenuBackgroundSceneSystem)->setEnabled(false);
//	}
}

void ClientConnectToServerSystem::connectToNetworkAddress()
{
	m_tcpClient->connectToServerAsync( m_serverAddress, m_serverPort);
	m_isLookingForConnection = true;
}

void ClientConnectToServerSystem::setConnectionAddress( const std::string& p_address, 
													   const std::string& p_port )
{
	m_serverAddress = p_address;
	m_serverPort = p_port;
}
