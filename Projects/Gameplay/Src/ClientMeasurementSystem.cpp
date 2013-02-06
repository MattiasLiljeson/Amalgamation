#include "ClientMeasurementSystem.h"
#include <AntTweakBarWrapper.h>

ClientMeasurementSystem::ClientMeasurementSystem()
	: EntitySystem( SystemType::ClientMeasurementSystem )
{
}

ClientMeasurementSystem::~ClientMeasurementSystem()
{
}

void ClientMeasurementSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->modifyTheRefreshRate(
		AntTweakBarWrapper::MEASUREMENT, 0.1f );

	vector<EntitySystem*> systems = m_world->getSystems()->getSystemList();
	for(unsigned int i=0; i<systems.size(); i++)
	{
		if(systems[i]->getInfo() != "")
		{
			AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
				AntTweakBarWrapper::MEASUREMENT, systems[i]->getInfo().c_str(),
				TwType::TW_TYPE_DOUBLE, &systems[i]->getTimeUsedPerSecond(),
				"group='Per second ratio'" );
			AntTweakBarWrapper::getInstance()->addReadOnlyVariable(
				AntTweakBarWrapper::MEASUREMENT, (systems[i]->getInfo()+" ").c_str(),
				TwType::TW_TYPE_DOUBLE, &systems[i]->getAverageExecutionTime(),
				"group='Average process execution time'" );
		}
	}
}