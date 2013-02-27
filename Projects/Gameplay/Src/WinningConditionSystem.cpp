#include "WinningConditionSystem.h"
#include "PlayerScore.h"
#include <algorithm>
#include <boost/bind/bind.hpp>
#include "NetworkSynced.h"
#include "PlayersWinLosePacket.h"
#include <TcpServer.h>
#include "ServerStateSystem.h"
#include "ChangeStatePacket.h"

WinningConditionSystem::WinningConditionSystem(TcpServer* p_server)
	: EntitySystem(SystemType::WinningConditionSystem, 2, ComponentType::PlayerScore,
	ComponentType::NetworkSynced)
{
	m_endTime = 0.0f;
	m_elapsedGameSessionTime = 0.0f;
	m_server = p_server;
}

WinningConditionSystem::~WinningConditionSystem()
{
}

void WinningConditionSystem::process()
{
	if(m_stateSystem->getStateDelta(ServerStates::INGAME) == EnumGameDelta::ENTEREDTHISFRAME){
		m_elapsedGameSessionTime = 0.0f;
	}

	if(m_stateSystem->getCurrentState() == ServerStates::INGAME){
		float dt = m_world->getDelta();
		m_elapsedGameSessionTime += dt;
		if(m_elapsedGameSessionTime >= m_endTime)
		{
			m_stateSystem->setQueuedState(ServerStates::RESULTS);
			ChangeStatePacket changeState;
			changeState.m_serverState = ServerStates::RESULTS;
			m_server->broadcastPacket(changeState.pack());
			
			m_endTime = 0.0f;
			m_elapsedGameSessionTime = 0.0f;
			m_enabled = false; // Disable this system.
			vector< pair<float, Entity*> > sorted =
				createSortedScoreEntityMapping();
			signalEndSession(sorted);
		}
	}
}

vector< pair<float, Entity*> > WinningConditionSystem::createSortedScoreEntityMapping()
{
	vector< pair<float, Entity*> > scoreEntityMapping;
	vector<Entity*> scoreEntities = getActiveEntities();
	for(unsigned int i=0; i<scoreEntities.size(); i++)
	{
		PlayerScore* score = static_cast<PlayerScore*>(scoreEntities[i]->getComponent(
			ComponentType::PlayerScore));
		scoreEntityMapping.push_back(pair<float, Entity*>(score->getScore(),
			scoreEntities[i]));
	}
	// Sort score with the highest first.
	std::sort(scoreEntityMapping.begin(), scoreEntityMapping.end(),
		boost::bind(&pair<float, Entity*>::first, _1) >
		boost::bind(&pair<float, Entity*>::first, _2));
	return scoreEntityMapping;
}

void WinningConditionSystem::signalEndSession(
	vector< pair<float, Entity*> > p_scoreComponentMapping)
{
	PlayersWinLosePacket winLosePacket;
	winLosePacket.activePlayers = p_scoreComponentMapping.size();
	for(unsigned int i=0; i<p_scoreComponentMapping.size(); i++)
	{
		NetworkSynced* netSync = static_cast<NetworkSynced*>(
			p_scoreComponentMapping[i].second->getComponent(ComponentType::NetworkSynced));
		winLosePacket.playerIdentities[i] = netSync->getNetworkOwner();
		winLosePacket.scores[i] = p_scoreComponentMapping[i].first;
	}

	calculateWinners(&winLosePacket);

	m_server->broadcastPacket(winLosePacket.pack());
}

void WinningConditionSystem::setEndTime( float p_endTime )
{
	m_endTime = p_endTime;
}

void WinningConditionSystem::initialize()
{
	m_stateSystem = static_cast<ServerStateSystem*>(
		m_world->getSystem(SystemType::ServerStateSystem));
}

void WinningConditionSystem::calculateWinners(PlayersWinLosePacket* p_packet)
{
	int index = 0;
	// No need to calculate who is the winner if the number of active players is 1
	if(p_packet->activePlayers > 1){
		do 
		{
			if(p_packet->scores[index] == p_packet->scores[index+1]){
				p_packet->winner[index+1] = true;
			}
			index++;
		} while (index+1 <= p_packet->activePlayers && p_packet->winner[index]);
	}
	else{
		p_packet->winner[0] = true;
	}
}
