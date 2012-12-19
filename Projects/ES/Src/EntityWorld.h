#pragma once

/**
 * The primary instance for the framework. It contains all the managers.
 * 
 * You must use this to create, delete and retrieve entities.
 * 
 * It is also important to set the delta each game loop iteration, and initialize before game loop.
 */

#include "ComponentManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "EntitySystem.h"
#include "IEntityObserver.h"
#include "IPerformer.h"
#include "SystemManager.h"
#include "SystemType.h"
#include <vector>
#include <map>

using namespace std;

class ComponentManager;
class Entity;
class EntityManager;
class EntitySystem;
class IEntityObserver;
class IPerformer;
class SystemManager;
class SystemType;

class EntityWorld
{
public:
	EntityWorld();
	~EntityWorld();

	/**
	 * Makes sure all managers systems are initialized in the order they were added.
	 */
	void initialize();
	
	/**
	 * Returns a manager that takes care of all the entities in the world.
	 * entities of this world.
	 * 
	 * @return entity manager.
	 */
	EntityManager* getEntityManager();
	
	/**
	 * Returns a manager that takes care of all the components in the world.
	 * 
	 * @return component manager.
	 */
	ComponentManager* getComponentManager();
	
	/**
	 * Add a manager into this world. It can be retrieved later.
	 * World will notify this manager of changes to entity.
	 * 
	 * @param p_managerType type of the manager specified by enum in Manager class
	 * @param p_manager to be added
	 */
	Manager* setManager( Manager::ManagerTypeIdx p_managerType, Manager* p_manager );

	/**
	 * Returns a manager of the specified type.
	 * 
	 * @param p_managerType type of the manager specified by enum in Manager class
	 * @return the manager
	 */
	Manager* getManager( Manager::ManagerTypeIdx p_managerType );

	/**
	 * Deletes the manager from this world.
	 * @param p_managerType type of the manager specified by enum in Manager class
	 */
	void deleteManager( Manager* p_manager );
	void deleteManager( Manager::ManagerTypeIdx p_managerType );

	/// Get the number of seconds elapsed since the start of the application
	/// @returns Seconds elapsed
	float getElapsedTime();

	/**
	 * Time since last game loop.
	 * 
	 * @return delta time since last game loop.
	 */
	float getDelta();

	/**
	 * You must specify the delta for the game here.
	 * 
	 * @param p_dt time since last game loop.
	 */
	void setDelta( float p_dt );

	/**
	 * Adds a entity to this world.
	 * 
	 * @param p_entity entity
	 */
	void addEntity( Entity* p_entity );

	/**
	 * Ensure all systems are notified of changes to this entity.
	 * If you're adding a component to an entity after it's been
	 * added to the world, then you need to invoke this method.
	 * 
	 * @param p_entity entity
	 */
	void changedEntity( Entity* p_entity );

	/**
	 * Delete the entity from the world.
	 * 
	 * @param p_entity entity
	 */
	void deleteEntity( Entity* p_entity );
	
	/**
	 * (Re)enable the entity in the world, after it having being disabled.
	 * Won't do anything unless it was already disabled.
	 */
	void enable( Entity* p_entity );

	/**
	 * Disable the entity from being processed. Won't delete it, it will
	 * continue to exist but won't get processed.
	 */
	void disable( Entity* p_entity );

	/**
	 * Create and return a new or reused entity instance.
	 * Will NOT add the entity to the world, use World.addEntity(Entity) for that.
	 * 
	 * @return entity
	 */
	Entity* createEntity();

	/**
	 * Get a entity having the specified id.
	 * 
	 * @param p_entityId
	 * @return entity
	 */
	Entity* getEntity( int p_entityId );

	/**
	 * Gives you all the systems in this world for possible iteration.
	 * 
	 * @return all entity systems in world.
	 */
	SystemManager* getSystems();

	/**
	 * Will add a system to this world.
	 *  
	 * @param p_type Type of system.
	 * @param p_system The system to add.
	 * @param p_enabled Wether or not this system will be processed by World.process().
	 * Defaults to true.
	 * @return The added system.
	 */
	EntitySystem* setSystem( SystemType p_type, EntitySystem* p_system,
		bool p_enabled = false );

	/**
	 * Will add a system to this world.
	 *  
	 * @param p_typeIdx Index for the type of system.
	 * @param p_system The system to add.
	 * @param p_enabled Wether or not this system will be processed by World.process().
	 * Defaults to true.
	 * @return The added system.
	 */
	EntitySystem* setSystem( SystemType::SystemTypeIdx p_typeIdx, EntitySystem* p_system,
		bool p_enabled = false );

	EntitySystem* setSystem( EntitySystem* p_system,
		bool p_enabled = false );

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

	//void notifySystems( IPerformer* p_performer, Entity* p_entity );
	
	void notifyManagers( IPerformer* performer, Entity* p_entity );

	/**
	 * Retrieve a system for specified system type.
	 * 
	 * @param p_type type of system.
	 * @return instance of the system in this world.
	 */
	EntitySystem* getSystem( SystemType p_type );
	EntitySystem* getSystem( SystemType::SystemTypeIdx p_typeIdx );

	/**
	 * Performs an action on each entity.
	 * @param entities
	 * @param performer
	 */
	void check( vector<Entity*>& p_entities, IPerformer* p_performer );

	void process();

	//<T extends Component> ComponentMapper<T> getMapper(Class<T> type);

private:
	//void deleteSystemFromBag(EntitySystem* system);

private:
	float m_delta;
	float m_totalGameTime; ///< number of seconds elapsed since the start of the application
	EntityManager* m_entityManager;
	ComponentManager* m_componentManager;
	SystemManager* m_systemManager;

	vector<Entity*> m_added;	
	vector<Entity*> m_changed;
	vector<Entity*> m_deleted;
	vector<Entity*> m_enable;
	vector<Entity*> m_disable;

	vector<Manager*> m_managers;

	// From Java, needed?
	vector<Manager*> m_managersBag;
};

