#pragma once

// =======================================================================================
//                                      EntitySystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief The most raw entity system.
///        
/// # EntitySystem
/// It should not typically be used, but you can create your 
/// own entity system handling by extending this. It is recommended that you use the other
/// provided entity system implementations.
/// Created on: 28-11-2012 
///---------------------------------------------------------------------------------------

class EntitySystem
{
private:
public:
	EntitySystem();
	virtual ~EntitySystem();
};

#endif // ENTITYSYSTEM_H

#include "Entity.h"
#include "EntityWorld.h"
#include "ComponentType.h"
#include <map>
#include <stdarg.h>

using namespace std;

class EntityWorld;
class Entity;

class EntitySystem
{
public:
	EntitySystem();

	/**
	 * Creates an entity system that uses the specified components as a matcher against 
	 * entities.
	 * @param number of components 
	 * @param components to match against entities
	 */
	EntitySystem( int p_numComponents, ... );
	~EntitySystem();

	void setSystemBits( bitset<SystemType::NUM_SYSTEM_TYPES> p_bits );

	/**
	 * Called before processing of entities begins. 
	 */
	virtual void begin();
	
	/**
	 * Called before processing of entities begins. 
	 */
	virtual void process();

	/**
	 * Called after the processing of entities ends.
	 */
	virtual void end();

	/**
	 * Any implementing entity system must implement this method and the logic
	 * to process the given entities of the system.
	 * 
	 * @param entities the entities this system contains.
	 */
	virtual void processEntities(const map<int, Entity*>& p_entities );
	
	/**
	 * 
	 * @return true if the system should be processed, false if not.
	 */
	virtual bool checkProcessing();
	
	/**
	 * Override to implement code that gets executed when systems are initialized.
	 */
	virtual void initialize();
	
	/**
	 * Called if the system has received a entity it is interested in, e.g. created or a component was added to it.
	 * @param e the entity that was added to this system.
	 */
	virtual void onAdded(Entity* e);
	
	/**
	 * Called if a entity was removed from this system, e.g. deleted or had one of it's components removed.
	 * @param e the entity that was removed from this system.
	 */
	virtual void onRemoved(Entity* e);

	virtual void onEnabled(Entity* e);
	virtual void onDisabled(Entity* e);
	virtual void onChange(Entity* e);

	void add(Entity* e);
	void remove(Entity* e);
	void enable(Entity* e);
	void disable(Entity* e);

	EntityWorld* getWorld() const;
	void setWorld( EntityWorld* p_world );

	void toggle();
	bool getEnabled() const;
	void setEnabled( bool p_enabled );


protected:
	EntityWorld* m_world;
	bool m_enabled;

private:
	map< int, Entity* > m_actives;

	bitset<SystemType::NUM_SYSTEM_TYPES> m_systemBits;
	bitset<ComponentType::NUM_COMPONENT_TYPES> m_componentBits;
};

