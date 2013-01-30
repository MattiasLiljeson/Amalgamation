#include "WinningConditionSystem.h"

WinningConditionSystem::WinningConditionSystem(TcpServer* p_server)
	: EntitySystem(SystemType::WinningConditionSystem, 0)
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
		signalEndSession();
	}
}

void WinningConditionSystem::startGameSession(float p_endTime)
{
	m_endTime = p_endTime;
	m_elapsedGameSessionTime = 0.0f;
	m_enabled = true; // Enable this system.
}

void WinningConditionSystem::signalEndSession()
{
}