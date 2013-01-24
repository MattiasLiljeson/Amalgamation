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

#include "ComponentType.h"
#include "Entity.h"
#include "EntityWorld.h"
#include "IEntityObserver.h"
#include "SystemType.h"
#include <map>
#include <stdarg.h>

using namespace std;

class ComponentType;
class Entity;
class EntityWorld;
class IEntityObserver;
class SystemType;

class EntitySystem : public IEntityObserver
{
public:
	EntitySystem( SystemType::SystemTypeIdx p_type );

	/**
	 * Creates an entity system that uses the specified components as a matcher against 
	 * entities.
	 * @param p_type Type of system.
	 * @param p_numComponents Number of components 
	 * @param ... Components to match against entities
	 */
	EntitySystem( SystemType::SystemTypeIdx p_type, int p_numComponents, ... );
	virtual ~EntitySystem();

	void setSystemBits( bitset<SystemType::NUM_SYSTEM_TYPES> p_bits );

	SystemType getSystemType();
	SystemType::SystemTypeIdx getSystemTypeIdx();

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
	virtual void processEntities( const vector<Entity*>& p_entities );

	/**
	 * 
	 * 
	 * @return a reference to the active entities that are stored in a vector.
	 */
	const vector<Entity*>& getActiveEntities() const;
	
	/**
	 * 
	 * @return true if the system should be processed, false if not.
	 */
	virtual bool checkProcessing();
	
	/**
	 * Override to implement code that gets executed when systems are initialized.
	 */
	virtual void initialize();
	
	// From C#
	///**
	// * Called if the system has received a entity it is interested in, e.g. created or a
	// * component was added to it.
	// * @param e the entity that was added to this system.
	// */
	//virtual void onAdded(Entity* e);
	//
	///**
	// * Called if a entity was removed from this system, e.g. deleted or had one of it's
	// * components removed.
	// * @param e the entity that was removed from this system.
	// */
	//virtual void onRemoved(Entity* e);
	//
	//virtual void onEnabled(Entity* e);
	//virtual void onDisabled(Entity* e);
	//virtual void onChange(Entity* e);
	//void add(Entity* e);
	//void remove(Entity* e);
	//void enable(Entity* e);
	//void disable(Entity* e);

	/**
	 * Will check if the entity is of interest to this system.
	 * @param p_entity Entity to check
	 */
	void check( Entity* p_entity );

	/**
	 * Called if the system has received a entity it is interested in, e.g. created or a component was added to it.
	 * @param p_entity The entity that was added to this system.
	 */
	virtual void inserted( Entity* p_entity ) {};

	/**
	 * Called if a entity was removed from this system, e.g. deleted or had one of it's components removed.
	 * @param p_entity The entity that was removed from this system.
	 */
	virtual void removed( Entity* p_entity ) {};

	void removeFromSystem( Entity* p_entity );
	void insertToSystem( Entity* p_entity );

	void added( Entity* p_entity );
	void changed( Entity* p_entity );
	void deleted( Entity* p_entity );
	void enabled( Entity* p_entity );
	void disabled( Entity* p_entity );

	EntityWorld* getWorld() const;
	void setWorld( EntityWorld* p_world );

	void toggle();
	bool getEnabled() const;
	void setEnabled( bool p_enabled );


	///-----------------------------------------------------------------------------------
	/// Desc Used for testing purposes. Returns a bitset representing the
	/// Component combinations used by the system.
	/// \return bitset<SystemType::NUM_SYSTEM_TYPES>
	///-----------------------------------------------------------------------------------
	bitset<ComponentType::NUM_COMPONENT_TYPES> getComponentBits();

protected:
	int findEntityInActive( Entity* p_entity );
	EntityWorld* m_world;
	bool m_enabled;

private:
	vector<Entity*> m_actives;

	//bitset<SystemType::NUM_SYSTEM_TYPES> m_systemBits;
	SystemType m_type;
	bitset<ComponentType::NUM_COMPONENT_TYPES> m_componentBits;
};

