#include "ClientConnectToServerSystem.h"
#include "Control.h"
#include <TcpClient.h>
#include "InputBackendSystem.h"
#include "LevelHandlerSystem.h"
#include "GameState.h"
#include "LibRocketEventManagerSystem.h"

ClientConnectToServerSystem::ClientConnectToServerSystem(TcpClient* p_tcpClient,
														 bool p_connectDirectly/* =false */)
	: EntitySystem( SystemType::ClientConnectoToServerSystem , 1, ComponentType::GameState)
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

void ClientConnectToServerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if( m_tcpClient->hasActiveConnection() )
	{
		m_isLookingForConnection = false;
		m_world->getSystem(SystemType::ClientPacketHandlerSystem)->setEnabled(true);
		setEnabled(false);

		auto eventManagerSys = static_cast<LibRocketEventManagerSystem*>(
			m_world->getSystem(SystemType::LibRocketEventManagerSystem));

		// Clears and hides all currently visible documents.
		//eventManagerSys->clearDocumentStack();
		
		auto levelHandler = static_cast<LevelHandlerSystem*>(
			m_world->getSystem(SystemType::LevelHandlerSystem));
		levelHandler->destroyLevel();
	}
	else if(p_entities.size() > 0)
	{
		auto state = static_cast<GameState*>(p_entities[0]->getComponent(
			ComponentType::GameState));

		InputBackendSystem* inputBackend = static_cast<InputBackendSystem*>
			(m_world->getSystem(SystemType::InputBackendSystem));

		if( state->getStateDelta(LOBBY) == ENTEREDTHISFRAME){
			connectToNetworkAddress();
			// Disable the menu background scene.
			//m_world->getSystem(SystemType::MenuBackgroundSceneSystem)->setEnabled(false);
		}

		if(inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys_F4) > 0 ){
			connectToNetworkAddress();
			state->setStatesDelta(INGAME, ENTEREDTHISFRAME);
			// Disable the menu background scene.
			m_world->getSystem(SystemType::MenuBackgroundSceneSystem)->setEnabled(false);
		}
	}
}

void ClientConnectToServerSystem::connectToNetworkAddress()
{
	m_tcpClient->connectToServerAsync( m_serverAddress, m_serverPort);
	m_isLookingForConnection = true;
}

/*
void ClientConnectToServerSystem::connectToNetworkAddress(
														const std::string& p_serverAddress, 
														const std::string& p_serverPort)
{
	m_tcpClient->connectToServerAsync( p_serverAddress, p_serverPort);
	m_isLookingForConnection = true;
}
*/


void ClientConnectToServerSystem::setConnectionAddress( const std::string& p_address, 
													   const std::string& p_port )
{
	m_serverAddress = p_address;
	m_serverPort = p_port;
}
