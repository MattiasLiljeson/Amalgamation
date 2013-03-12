#include "ClientEntityCountSystem.h"
#include <AntTweakBarWrapper.h>
#include <PreciseTimer.h>

ClientEntityCountSystem::ClientEntityCountSystem()
	: EntitySystem( SystemType::ClientEntityCountSystem )
{
	m_entitiesTotal = 0;
	m_entitiesInserted = 0;
	m_entitiesRemoved = 0;
	m_entitiesInsertedLastSecond = 0;
	m_entitiesRemovedLastSecond = 0;
	m_secondTimer = new PreciseTimer();
}

ClientEntityCountSystem::~ClientEntityCountSystem()
{
	delete m_secondTimer;
}

void ClientEntityCountSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::OVERALL,
		"Entities total", TwType::TW_TYPE_UINT32, &m_entitiesTotal,
		"group='Entities'");
//	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::OVERALL,
//		"Inserted count", TwType::TW_TYPE_UINT32, &m_entitiesInsertedLastSecond,
//		"group='Entities'");
//	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::OVERALL,
//		"Removed count", TwType::TW_TYPE_UINT32, &m_entitiesRemovedLastSecond,
//		"group='Entities'");
	m_secondTimer->start();
}

void ClientEntityCountSystem::process()
{
	if(m_secondTimer->elapsed() > 1.0)
	{
		m_entitiesTotal = static_cast<unsigned int>(
			m_world->getEntityManager()->getActiveEntityCount());
		m_entitiesInsertedLastSecond = m_entitiesInserted;
		m_entitiesInserted = 0;
		m_entitiesRemovedLastSecond = m_entitiesRemoved;
		m_entitiesRemoved = 0;
		m_secondTimer->stop();
		m_secondTimer->start();
	}
}

void ClientEntityCountSystem::added( Entity* p_entity )
{
	m_entitiesInserted += 1;
}

void ClientEntityCountSystem::deleted( Entity* p_entity )
{
	m_entitiesRemoved += 1;
}