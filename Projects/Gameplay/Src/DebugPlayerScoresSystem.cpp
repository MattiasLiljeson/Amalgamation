#include "DebugPlayerScoresSystem.h"

#include "PlayerScore.h"
#include <DebugUtil.h>

#include <sstream>
#include <string>
#include <iostream>
using namespace std;

DebugPlayerScoresSystem::DebugPlayerScoresSystem()
	: EntitySystem( SystemType::DebugPlayerScoresSystem,
	1, ComponentType::PlayerScore )
{
	m_timer = m_timerResetValue = 1.0f;
}

DebugPlayerScoresSystem::~DebugPlayerScoresSystem()
{
}

void DebugPlayerScoresSystem::initialize()
{
}

void DebugPlayerScoresSystem::processEntities( const vector<Entity*>& p_entities )
{
	m_timer -= m_world->getDelta();

	if( m_timer <= 0 )
	{
		m_timer = m_timerResetValue;

		stringstream ss;
		ss << "";

		unsigned int numEntities = p_entities.size();
		for(unsigned int i=0; i<numEntities; i++)
		{
			ss << static_cast<PlayerScore*>(p_entities[i]->getComponent(
				ComponentType::PlayerScore ))->getScore();
			if( i < numEntities - 1 )
				ss << ", ";
		}
	}
}