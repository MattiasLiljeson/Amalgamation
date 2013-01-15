#include "ServerPacketHandlerSystem.h"
#include "ServerPickingSystem.h"
#include "ShipModulesControllerSystem.h"

// Components
#include "Transform.h"
#include "NetworkSynced.h"
#include "PhysicsBody.h"
#include "ShipModule.h"
#include "SpeedBoosterModule.h"

// NetComm
#include <TcpServer.h>

#include "PacketType.h"
#include "EntityType.h"
#include "PhysicsSystem.h"
#include "TimerSystem.h"
#include "ThrustPacket.h"
#include "PingPacket.h"
#include "PongPacket.h"
#include "RayPacket.h"
#include "UpdateClientStatsPacket.h"
#include "HighlightSlotPacket.h"
#include "SimpleEventPacket.h"

ServerPacketHandlerSystem::ServerPacketHandlerSystem( TcpServer* p_server )
	: EntitySystem( SystemType::ServerPacketHandlerSystem, 3,
	ComponentType::NetworkSynced, ComponentType::ShipFlyController,
	ComponentType::PhysicsBody )
{
	m_server = p_server;
}

ServerPacketHandlerSystem::~ServerPacketHandlerSystem()
{

}


void ServerPacketHandlerSystem::initialize()
{
	m_physics = static_cast<PhysicsSystem*>(
		m_world->getSystem( SystemType::PhysicsSystem ) );
}

void ServerPacketHandlerSystem::processEntities( const vector<Entity*>& p_entities )
{
	while( m_server->hasNewPackets() )
	{
		Packet packet = m_server->popNewPacket();

		char packetType;
		packetType = packet.getPacketType();
		
		if(packetType == (char)PacketType::ThrustPacket)
		{
			ThrustPacket thrustPacket;
			thrustPacket.unpack( packet );

			PhysicsBody* physicsBody = static_cast<PhysicsBody*>
				(m_world->getEntity(thrustPacket.entityId)->getComponent(
				ComponentType::PhysicsBody));

			//Added by Anton
			Entity* ship = m_world->getEntity(thrustPacket.entityId);

			ConnectionPointSet* connected =
				static_cast<ConnectionPointSet*>(
				m_world->getComponentManager()->getComponent(ship,
				ComponentType::getTypeFor(ComponentType::ConnectionPointSet)));
			
			AglVector3 boostVector = AglVector3(0, 0, 0);
			if (connected->m_connectionPoints[connected->m_highlighted].cpConnectedEntity >= 0)
			{
				Entity* shipModule = m_world->getEntity(connected->m_connectionPoints[connected->m_highlighted].cpConnectedEntity);
				ShipModule* module = static_cast<ShipModule*>(shipModule->getComponent(ComponentType::ShipModule));
				SpeedBoosterModule* boostmodule = static_cast<SpeedBoosterModule*>(shipModule->getComponent(ComponentType::SpeedBoosterModule));
				if (module->m_active && boostmodule)
					boostVector = thrustPacket.thrustVector*3;
			}

			m_physics->applyImpulse( physicsBody->m_id, thrustPacket.thrustVector+boostVector,
				thrustPacket.angularVector );
		}
		if( packetType == (char)PacketType::Ping )
		{
			PingPacket pingPacket;
			pingPacket.unpack( packet );

			Packet response((char)PacketType::Pong);
			response << pingPacket.timeStamp;

			m_server->unicastPacket( response, packet.getSenderId() );
		}
		else if( packetType == (char)PacketType::Pong)
		{
			ClientInfo info;
			float totalElapsedTime = m_world->getElapsedTime();
			float timeWhenSent;

			PongPacket pongPacket;
			pongPacket.unpack( packet );
			timeWhenSent = pongPacket.timeStamp;

			/************************************************************************/
			/* Convert from seconds to milliseconds.								*/
			/************************************************************************/
			info.ping = (totalElapsedTime - timeWhenSent)*1000.0f;
			m_clients[packet.getSenderId()] = info;

			/************************************************************************/
			/* Send the "real" ping back to the client as a "your ping" message.    */
			/************************************************************************/
			UpdateClientStatsPacket updatedClientPacket;
			updatedClientPacket.ping = info.ping;
			updatedClientPacket.currentServerTimestamp = m_world->getElapsedTime();
			m_server->unicastPacket(updatedClientPacket.pack(), packet.getSenderId());
		}	
		else if (packetType == (char)PacketType::RayPacket)
		{
			ServerPickingSystem* picking = 
				static_cast<ServerPickingSystem*>(m_world->getSystem(SystemType::ServerPickingSystem));

			RayPacket rayPacket;
			rayPacket.unpack( packet );
			picking->setRay(packet.getSenderId(), rayPacket.o, rayPacket.d);
		}
		else if (packetType == (char)PacketType::ModuleHighlightPacket)
		{
			ShipModulesControllerSystem* modsystem = 
				static_cast<ShipModulesControllerSystem*>(m_world->getSystem(SystemType::ShipModulesControllerSystem));

			HighlightSlotPacket hp;
			hp.unpack( packet );
			modsystem->addHighlightEvent(hp.id, packet.getSenderId());
		}
		else if (packetType == (char)PacketType::SimpleEvent)
		{
			ShipModulesControllerSystem* modsystem = 
				static_cast<ShipModulesControllerSystem*>(m_world->getSystem(SystemType::ShipModulesControllerSystem));

			SimpleEventPacket sep;
			sep.unpack( packet );
			if (sep.type == SimpleEventType::ACTIVATE_MODULE)
				modsystem->addActivateEvent(packet.getSenderId());
			else if (sep.type == SimpleEventType::DEACTIVATE_MODULE)
				modsystem->addDeactivateEvent(packet.getSenderId());
		}
	}
	
	if( static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::HalfSecond))
	{
		float timeStamp = m_world->getElapsedTime();

		PingPacket pingPacket;
		pingPacket.timeStamp = timeStamp;

		m_server->broadcastPacket( pingPacket.pack() );
	}
}
