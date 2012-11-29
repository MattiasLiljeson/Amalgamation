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
#include "IPerformer.h"
#include "SystemManager.h"
#include <vector>
#include <map>

using namespace std;

class ComponentManager;
class Entity;
class EntityManager;
class SystemManager;

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
	Manager* setManager( Manager::ManagerType p_managerType, Manager* p_manager );

	/**
	 * Returns a manager of the specified type.
	 * 
	 * @param p_managerType type of the manager specified by enum in Manager class
	 * @return the manager
	 */
	Manager* getManager( Manager::ManagerType p_managerType );

	/**
	 * Deletes the manager from this world.
	 * @param p_managerType type of the manager specified by enum in Manager class
	 */
	void deleteManager( Manager* p_manager );
	void deleteManager( Manager::ManagerType p_managerType );

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
	 * @param entityId
	 * @return entity
	 */
	Entity* getEntity( int p_entityId );

	// waddabout system manager?
	ImmutableBag<EntitySystem> getSystems();
	<T extends EntitySystem> T setSystem(T system);
	<T extends EntitySystem> T setSystem(T system, boolean passive);
	void deleteSystem(EntitySystem system);
	void notifySystems(Performer performer, Entity* p_entity );
	
	void notifyManagers( IPerformer* performer, Entity* p_entity );

	/**
	 * Retrieve a system for specified system type.
	 * 
	 * @param type type of system.
	 * @return instance of the system in this world.
	 */
	EntitySystem* getSystem( SystemType p_type );
	EntitySystem* getSystem( SystemType::SystemTypeIdx p_typeIdx );

	/**
	 * Performs an action on each entity.
	 * @param entities
	 * @param performer
	 */
	void check( vector<Entity*> p_entities, IPerformer* p_performer );

	void process();

	//<T extends Component> ComponentMapper<T> getMapper(Class<T> type);

private:
	float m_delta;
	EntityManager* m_entityManager;
	ComponentManager* m_componentManager;
	SystemManager* m_systemManager;

	vector<Entity*> m_added;	
	vector<Entity*> m_changed;
	vector<Entity*> m_deleted;
	vector<Entity*> m_enable;
	vector<Entity*> m_disable;

	vector<Manager*> m_managers;
	vector<EntitySystem*> m_systems;

	// From Java, needed?
	vector<Manager*> m_managersBag;
	vector<EntitySystem*> m_systemsBag;
	
};

