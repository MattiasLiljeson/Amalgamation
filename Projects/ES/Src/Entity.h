#pragma once

#include "ComponentType.h"
#include "EntityWorld.h"
#include "SystemType.h"
#include "ComponentManager.h"

#include "Tag.h" // Added by Jarl 2013-01-09, temporary management of tags
#include <string>

class ComponentType;
class EntityWorld;
class SystemType;

class Entity
{
public:

	/** Entity constructor. DO NOT call this yourself. This is done by the world 
	 * @param p_world pointer to the world its being bound to.
	 * @param p_index The index this Entity will use with the world. 
	 */
	Entity( EntityWorld* p_world, int p_index );

	~Entity(void);

	void reset();

	/** Returns the globally unique ID. NOT IMPLEMENTED!
	 * This is the ID that shall be used in all game logic as it's unique for every entity
	 * for this game session. This is also unique across the network.
	 */
	int getUUID();

	/** Returns the Entities world-index. 
	 * This is the index that is used for storing Components etc. These are reused between
	 * Entities if they are destroyed. use this if you want to fetch data about the 
	 * entity.
	 */
	int getIndex();

	/** The world the Entity has been bound to.
	 * @return The world the Entity has been bound to.
	 */
	EntityWorld* getWorld();

	bitset<ComponentType::NUM_COMPONENT_TYPES> getComponentBits();
	void setComponentBits( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits );
	void setComponentBit( int p_idx, bool p_value );
	void addComponentBit( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits );
	void removeComponentBit( bitset<ComponentType::NUM_COMPONENT_TYPES> p_componentBits );

	/** Shorthand function for adding components to the Entity.
	 * Will use the worlds ComponentManager to add the component. The component manager
	 * will take ownership of the component and delete it when the entity dies.
	 *
	 * @param p_typeIdx Index for the type of component to add. Used as index in the
	 * component database in the worlds ComponentManager.
	 *
	 * @param The new component you wish to add.
	 */
	void addComponent( Component* p_component );
	void addComponent( ComponentType::ComponentTypeIdx p_typeIdx, Component* p_component );
	void addComponent( ComponentType p_type, Component* p_component );

	///-----------------------------------------------------------------------------------
	/// Temporary hadnling of tags. Tags are right now basically just a component
	/// However this special interfacing add function was implemented to clarify that there
	/// should be a difference later on. (Added by Jarl, 2013-01-09)
	/// \param p_typeIdx
	/// \param p_tag
	/// \return void
	///-----------------------------------------------------------------------------------
	void addTag( ComponentType::ComponentTypeIdx p_typeIdx, Tag* p_tag );


	///-----------------------------------------------------------------------------------
	/// Shorthand function for removing components for the entity
	/// \param p_typeIdx
	/// \param p_component
	/// \return void
	///-----------------------------------------------------------------------------------
	void removeComponent( ComponentType::ComponentTypeIdx p_typeIdx );
	void removeComponent( ComponentType p_type );

	void applyComponentChanges();
	

	///\brief Shorthand function for fetching a components component from the worlds 
	/// component manager
	///\param p_typeIdx The type of component wished for
	///\return The entity's component of the supplied ComponentType 
	Component* getComponent( ComponentType::ComponentTypeIdx p_typeIdx );

	bitset<SystemType::NUM_SYSTEM_TYPES> getSystemBits();
	void setSystemBits( bitset<SystemType::NUM_SYSTEM_TYPES> p_systemBits );
	void setSystemBit( int p_idx, bool p_value );
	void addSystemBit( bitset<SystemType::NUM_SYSTEM_TYPES> p_systemBits );
	void removeSystemBit( bitset<SystemType::NUM_SYSTEM_TYPES> p_systemBits );

	bool isEnabled();
	void setEnabled( bool p_enabled );

	void setName( string p_name ) { m_name = p_name; }
	const string& getName() const;

private:
	// Universal Unique ID. This id is unique in the network and will never be reused. 
	int m_UUID; ///<NOT IMPLEMENTED!

	// This id  is used as an index for the components internally. If an entity is deleted
	// from the world its id will be reused for future created entities.
	int m_index;

	/// Used for debug purposes
	string m_name; 

	EntityWorld* m_world;

	bool m_enabled;

	bitset<ComponentType::NUM_COMPONENT_TYPES> m_componentBits;
	bitset<SystemType::NUM_SYSTEM_TYPES> m_systemBits;
};

