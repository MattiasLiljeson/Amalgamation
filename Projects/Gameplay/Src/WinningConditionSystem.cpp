#include "WinningConditionSystem.h"
#include "PlayerScore.h"
#include <algorithm>
#include <boost/bind/bind.hpp>
#include "NetworkSynced.h"
#include "PlayersWinLosePacket.h"
#include <TcpServer.h>

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
	float dt = m_world->getDelta();
	m_elapsedGameSessionTime += dt;
	if(m_elapsedGameSessionTime >= m_endTime)
	{
		m_endTime = 0.0f;
		m_elapsedGameSessionTime = 0.0f;
		m_enabled = false; // Disable this system.
		vector< pair<float, Entity*> > sorted = createSortedScoreEntityMapping();
		signalEndSession(sorted);
	}
}

void WinningConditionSystem::startGameSession(float p_endTime)
{
	m_endTime = p_endTime;
	m_elapsedGameSessionTime = 0.0f;
	m_enabled = true; // Enable this system.
}

vector< pair<float, Entity*> > WinningConditionSystem::createSortedScoreEntityMapping()
{
	vector< pair<float, Entity*> > scoreEntityMapping;
	vector<Entity*> scoreEntities = getActiveEntities();
	for(unsigned int i=0; i<scoreEntities.size(); i++)
	{
		PlayerScore* score = static_cast<PlayerScore*>(scoreEntities[i]->getComponent(
			ComponentType::PlayerScore));
		scoreEntityMapping.push_back(pair<float, Entity*>(score->getTotalScore(),
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
		winLosePacket.playerIdentities[i] = p_scoreComponentMapping[i].second->getIndex();
		winLosePacket.scores[i] = p_scoreComponentMapping[i].first;
		winLosePacket.winner[i] = false;
	}
	if(!p_scoreComponentMapping.empty())
	{
		winLosePacket.winner[0] = true; // Set first place to winner.
	}
	m_server->broadcastPacket(winLosePacket.pack());
}