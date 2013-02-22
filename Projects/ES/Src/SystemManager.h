#pragma once

#include "EntitySystem.h"
#include "EntityWorld.h"
#include "Manager.h"
#include "SystemType.h"

class EntitySystem;
class EntityWorld;
class Manager;
class SystemType;
class PreciseTimer;

class SystemManager : public Manager
{
public:
	SystemManager( EntityWorld* p_world );
	~SystemManager();

	void initialize(){ initializeAll(); };
	EntitySystem* getSystem( SystemType::SystemTypeIdx p_systemIndex );

	/**
	 * Will add a system to this manager.
	 *  
	 * @param p_typeIdx Index for the type of system.
	 * @param p_system The system to add.
	 * @param p_enabled Wether or not this system will be processed by World.process().
	 * Defaults to true.
	 * @return The added system.
	 */
	EntitySystem* setSystem( SystemType::SystemTypeIdx p_typeIdx, EntitySystem* p_system,
		bool p_enabled = true );

	vector<SystemType::SystemTypeIdx> getSystemEnumList( Entity* p_entity );

	///-----------------------------------------------------------------------------------
	/// Get a reference to the system list, to be iterated over.
	/// \return const vector<EntitySystem*>&
	///-----------------------------------------------------------------------------------
	const vector<EntitySystem*>& getSystemList() const;

	/**
	 * Remove the specified system from the manager.
	 * @param p_type Type of system to be deleted from manager.
	 */
	void deleteSystem( SystemType p_type );

	/**
	 * Remove the specified system from the manager.
	 * @param p_typeIdx index of type of system to be deleted from manager.
	 */
	void deleteSystem( SystemType::SystemTypeIdx p_typeIdx );
	
	/**
	 * EXPENSIVE! Use the above methods if possible!
	 * Remove the specified system from the manager.
	 * @param p_system to be deleted from manager.
	 */
	void deleteSystem( EntitySystem* p_system);

	void initializeAll();
	void updateSynchronous();
	void notifySystems( IPerformer* p_performer, Entity* p_entity );
	const double& getTotalSystemExecutionTime() const;

private:
	map<SystemType::SystemTypeIdx, EntitySystem*> m_systems;

	// Vector that stores systems in the order they've been added. For dependencies
	vector<EntitySystem*> m_systemList;
	// For every system this vector contains a vector of every process() call execution time per second.
	vector< double > m_systemsExecutionTimeMeasurements;
	double m_totalSystemsExecutionTime;
	unsigned int m_tickCounter;
	double m_secondTimer;
	PreciseTimer* m_executionTimer;
};

