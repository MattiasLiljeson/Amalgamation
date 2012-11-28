#include "EntitySystem.h"

//bitset<ComponentType::NUM_COMPONENT_TYPES> EntitySystem::s_typeFlags;

EntitySystem::EntitySystem()
{
}

EntitySystem::EntitySystem( int p_numComponents, ... )
{
	va_list componentTypes;
	va_start( componentTypes, p_numComponents );

	for( int i=0; i<p_numComponents; i++ )
	{
		ComponentType::ComponentTypeIdx type = va_arg( componentTypes, ComponentType::ComponentTypeIdx );
		ComponentType ct = ComponentType::getTypeFor(type);
		m_componentBits |= ct.getBit();
	}
}

void EntitySystem::setSystemBits( bitset<SystemType::NUM_SYSTEM_TYPES> p_bits )
{
	m_systemBits = p_bits;
}

EntitySystem::~EntitySystem()
{
}

void EntitySystem::begin()
{

}

void EntitySystem::process()
{
	if (checkProcessing())
	{
		begin();
		processEntities(m_actives);
		end();
	}
}

void EntitySystem::end()
{

}

void EntitySystem::processEntities( const map<int, Entity*>& p_entities )
{

}

bool EntitySystem::checkProcessing()
{
	return true;
}

void EntitySystem::initialize()
{

}

void EntitySystem::onAdded( Entity* e )
{

}

void EntitySystem::onRemoved( Entity* e )
{

}

void EntitySystem::onEnabled( Entity* e )
{

}

void EntitySystem::onDisabled( Entity* e )
{

}

void EntitySystem::onChange( Entity* e )
{
	bool contains = ( m_systemBits & e->getSystemBits() ) == m_systemBits;
	bool interest = ( m_componentBits & e->getComponentBits() ) == m_componentBits;

	bitset<SystemType::NUM_SYSTEM_TYPES> empty;
	
	if (interest && !contains && m_systemBits != empty) {
		add(e);
	} else if (!interest && contains && m_systemBits != empty) {
		remove(e);
	} else if ((interest && contains && e->isEnabled()) && m_systemBits != empty) {
		enable(e);
	} else if ((interest && contains && e->isEnabled() == false) && m_systemBits != empty) {
		disable(e);
	}
}

void EntitySystem::add( Entity* e )
{
	e->addSystemBit(m_systemBits);
	if (e->isEnabled() == true) {
		enable(e);
	}
	onAdded(e);
}

void EntitySystem::remove( Entity* e )
{
	e->removeSystemBit(m_systemBits);
	if (e->isEnabled() == true) {
		disable(e);
	}
	onRemoved(e);
}

void EntitySystem::enable( Entity* e )
{
	// If the map contains the index
	if (m_actives.find(e->getIndex()) != m_actives.end()) {
		return;
	}
	m_actives[e->getIndex()] = e;
	onEnabled(e);
}

void EntitySystem::disable( Entity* e )
{
	// If the map DOESN'T contain the index
	if (m_actives.find(e->getIndex()) == m_actives.end()) {
		return;
	}
	m_actives.erase(e->getIndex());
	onDisabled(e);
}

EntityWorld* EntitySystem::getWorld() const
{
	return m_world;
}

void EntitySystem::setWorld( EntityWorld* p_world )
{
	m_world = p_world;
}

void EntitySystem::toggle()
{
	m_enabled = !m_enabled;
}

bool EntitySystem::getEnabled() const
{
	return m_enabled;
}

void EntitySystem::setEnabled( bool p_enabled )
{
	m_enabled = p_enabled;
}