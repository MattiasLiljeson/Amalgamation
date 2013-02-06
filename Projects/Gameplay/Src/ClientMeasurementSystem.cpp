#include "ClientMeasurementSystem.h"
#include <AntTweakBarWrapper.h>

ClientMeasurementSystem::ClientMeasurementSystem()
	: EntitySystem( SystemType::ClientMeasurementSystem )
{
}

ClientMeasurementSystem::~ClientMeasurementSystem()
{
}

void ClientMeasurementSystem::process()
{
}

void ClientMeasurementSystem::initialize()
{
	initMeasuredSystems();

	AntTweakBarWrapper::getInstance()->modifyTheRefreshRate(
		AntTweakBarWrapper::MEASUREMENT, 0.1f );
	AntTweakBarWrapper::getInstance()->defineParametersForABar(
		AntTweakBarStrings::measurement.c_str(), "mybar valueswidth=120");

	vector<EntitySystem*> systems = m_world->getSystems()->getSystemList();
	for(unsigned int i=0; i<m_measuredSystems.size(); i++)
	{
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT, m_measuredSystems[i].second.c_str(),
			TwType::TW_TYPE_DOUBLE, &m_measuredSystems[i].first->getTimeUsedPerSecond(),
			"group='Per second ratio' precision='6'" );
		AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
			AntTweakBarWrapper::MEASUREMENT, (m_measuredSystems[i].second+" ").c_str(),
			TwType::TW_TYPE_DOUBLE, &m_measuredSystems[i].first->getAverageExecutionTime(),
			"group='Average process execution time (ms)' precision='4'" );
	}
}

void ClientMeasurementSystem::initMeasuredSystems()
{
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::ClientPacketHandlerSystem),
		"ClientPacketHandler"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::InputBackendSystem),
		"InputBackend"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::GraphicsRendererSystem),
		"GraphicsRenderer"));
	m_measuredSystems.push_back(pair<EntitySystem*, string>(
		m_world->getSystem(SystemType::LibRocketBackendSystem),
		"LibRocket"));
}
