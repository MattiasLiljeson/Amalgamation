#include "SystemManager.h"
#include <PreciseTimer.h>
#include <ToString.h>
#include "OutputLogger.h"

SystemManager::SystemManager( EntityWorld* p_world )
{
	m_world = p_world;
	m_executionTimer = new PreciseTimer();
	m_secondTimer = 0.0;
	m_tickCounter = 0;
	m_totalSystemsExecutionTime = 0.0;
}

SystemManager::~SystemManager()
{
	map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
	{
		delete it->second;
		it->second = NULL;
	}
	delete m_executionTimer;
}

EntitySystem* SystemManager::getSystem( SystemType::SystemTypeIdx p_systemIndex )
{
	return m_systems[p_systemIndex];
}

EntitySystem* SystemManager::setSystem( SystemType::SystemTypeIdx p_typeIdx,
									   EntitySystem* p_system, bool p_enabled )
{
	//find and remove any previous versions of this system
	vector<EntitySystem*>::iterator it = m_systemList.begin();
	for( ; it!=m_systemList.end(); it++ )
	{
		if((*it)->getSystemTypeIdx() == p_typeIdx )
			it = m_systemList.erase(it);
	}
	m_systemList.push_back( p_system );
	m_systemsExecutionTimeMeasurements.push_back(0.0);	
	p_system->setEnabled( p_enabled );
	m_systems[p_typeIdx] = p_system;

	return p_system;
}

const vector<EntitySystem*>& SystemManager::getSystemList() const
{
	return m_systemList;
}

vector<SystemType::SystemTypeIdx> SystemManager::getSystemEnumList( Entity* p_entity )
{
	bitset<SystemType::NUM_SYSTEM_TYPES> systemBits =
		p_entity->getSystemBits();
	vector<SystemType::SystemTypeIdx> systemList;

	for(unsigned int i=0; i<systemBits.size(); i++ )
	{
		if( systemBits[i] == true)
		{
			systemList.push_back((SystemType::SystemTypeIdx)i);
		}
	}
	return systemList;
}

void SystemManager::deleteSystem( SystemType p_type )
{
	deleteSystem( (SystemType::SystemTypeIdx)p_type.getIndex() );
}

void SystemManager::deleteSystem( SystemType::SystemTypeIdx p_typeIdx )
{
	//find and remove the system
	vector<EntitySystem*>::iterator it = m_systemList.begin();
	for( ; it!=m_systemList.end(); it++ )
	{
		if((*it)->getSystemTypeIdx() == p_typeIdx )
			it = m_systemList.erase(it);
	}

	delete m_systems[p_typeIdx];
	m_systems[p_typeIdx] = NULL;
}

void SystemManager::deleteSystem( EntitySystem* p_system)
{
	int idx = p_system->getSystemTypeIdx();
	//find and remove the system
	for( vector<EntitySystem*>::iterator it = m_systemList.begin();
		it!=m_systemList.end();
		it++ )
	{
		if((*it)->getSystemTypeIdx() == idx )
			m_systemList.erase(it);
	}

	//NOTE: break in for-loop 
	map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	for( it=m_systems.begin(); it != m_systems.end(); it++ )
	{
		if( it->second == p_system)
		{
			delete p_system;
			it->second = NULL;
			break;
		}
	}
}

void SystemManager::initializeAll()
{
	// Should be done on the list. For dependency injection.
	//map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	//for( it=m_systems.begin(); it != m_systems.end(); it++ )
	//	it->second->initialize();

	for( unsigned int i=0; i<m_systemList.size(); i++ )
	{
		//m_world->getOutputLogger()->write( ("SystemManager begun initializing system of type " 
		//	+ toString((int)(m_systemList[i]->getSystemTypeIdx())) + "\n" ).c_str() );

		m_systemList[i]->initialize();

		//m_world->getOutputLogger()->write( ("SystemManager finished initializing system of type " 
		//	+ toString((int)(m_systemList[i]->getSystemTypeIdx())) + "\n" ).c_str() );
	}
}

void SystemManager::updateSynchronous()
{
	//map<SystemType::SystemTypeIdx, EntitySystem*>::iterator it;
	//for( it=m_systems.begin(); it != m_systems.end(); it++ )
	//	it->second->process();
	m_secondTimer += static_cast<double>(m_world->getDelta());
	m_tickCounter += 1;
	bool reset = m_secondTimer > 1.0;
	double executionTimeSum = 0.0;
	for( unsigned int i=0; i<m_systemList.size(); i++ )
	{
		EntitySystem* system = m_systemList[i];
		if( system->getEnabled() )
		{
			m_executionTimer->start();
			system->process();

			double elapsedTime = m_executionTimer->stop();
			m_systemsExecutionTimeMeasurements[i] += elapsedTime;
			m_systemList[i]->setLastExecutionTime(elapsedTime);
			executionTimeSum += elapsedTime;
			if(reset)
			{
				m_systemList[i]->setAverageExecutionTime(
					m_systemsExecutionTimeMeasurements[i] / (double)m_tickCounter
					* 1000.0);
				m_systemList[i]->setTimeUsedPerSecond(
					m_systemsExecutionTimeMeasurements[i]);
				m_systemsExecutionTimeMeasurements[i] = 0;
			}
		}
		else
		{
			m_systemList[i]->setAverageExecutionTime(0.0);
			m_systemList[i]->setTimeUsedPerSecond(0.0);
			m_systemList[i]->setLastExecutionTime(0.0);
		}
	}
	if(reset)
	{
		m_secondTimer = 0.0;
		m_tickCounter = 0;
	}
	m_totalSystemsExecutionTime = executionTimeSum;
}

void SystemManager::notifySystems( IPerformer* p_performer, Entity* p_entity )
{
	for( unsigned int i = 0; i<m_systemList.size(); i++ ) 
	{
		p_performer->perform(m_systemList[i], p_entity);
	}
}

const double& SystemManager::getTotalSystemExecutionTime() const
{
	return m_totalSystemsExecutionTime;
}