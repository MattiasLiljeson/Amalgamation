#include "ServerMeasurementSystem.h"
#include <DebugUtil.h>
#include <sstream>
#include <Windows.h>

ServerMeasurementSystem::ServerMeasurementSystem()
	: EntitySystem( SystemType::ServerMeasurementSystem )
{
}

ServerMeasurementSystem::~ServerMeasurementSystem()
{
}

void ServerMeasurementSystem::process()
{
	if(m_secondTimer.elapsed() > 1.0)
	{
		OutputDebugStringA( "----------------------------\n" );
		OutputDebugStringA( "- Per second ratio -\n" );
		for(unsigned int i=0; i<m_measuredSystems.size(); i++)
		{
			stringstream ss;
			ss << m_measuredSystems[i].second << ": ";
			ss << m_measuredSystems[i].first->getTimeUsedPerSecond() << "\n";
			OutputDebugStringA( ss.str().c_str() );
		}
		OutputDebugStringA( "- Process execution time (ms) -\n" );
		for(unsigned int i=0; i<m_measuredSystems.size(); i++)
		{
			stringstream ss;
			ss << m_measuredSystems[i].second << ": ";
			ss << m_measuredSystems[i].first->getAverageExecutionTime() << "\n";
			OutputDebugStringA( ss.str().c_str() );
		}
		m_secondTimer.stop();
		m_secondTimer.start();
	}
}

void ServerMeasurementSystem::initialize()
{
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::PhysicsSystem),
		"Physics"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::NetworkUpdateSystem),
		"NetworkUpdate"));

	m_secondTimer.start();
}