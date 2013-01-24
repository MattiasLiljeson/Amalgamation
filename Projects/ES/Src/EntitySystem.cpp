#include "EntitySystem.h"

//bitset<ComponentType::NUM_COMPONENT_TYPES> EntitySystem::s_typeFlags;

EntitySystem::EntitySystem( SystemType::SystemTypeIdx p_type )
{
	m_type = SystemType::getTypeFor(p_type);
}

EntitySystem::EntitySystem( SystemType::SystemTypeIdx p_type, int p_numComponents, ... )
{
	m_type = SystemType::getTypeFor(p_type);

	va_list componentTypes;
	va_start( componentTypes, p_numComponents );

	for( int i=0; i<p_numComponents; i++ )
	{
		ComponentType::ComponentTypeIdx type = va_arg( componentTypes, ComponentType::ComponentTypeIdx );
		ComponentType ct = ComponentType::getTypeFor(type);
		m_componentBits |= ct.getBit();
	}
}

//void EntitySystem::setSystemBits( bitset<SystemType::NUM_SYSTEM_TYPES> p_bits )
//{
//	m_systemBits = p_bits;
//}

SystemType EntitySystem::getSystemType()
{
	return m_type;
}

SystemType::SystemTypeIdx EntitySystem::getSystemTypeIdx()
{
	return m_type.getIndex();
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

void EntitySystem::processEntities( const vector<Entity*>& p_entities )
{

}

bool EntitySystem::checkProcessing()
{
	return true;
}

void EntitySystem::initialize()
{

}

//void EntitySystem::onAdded( Entity* e )
//{
//
//}
//
//void EntitySystem::onRemoved( Entity* e )
//{
//
//}
//
//void EntitySystem::onEnabled( Entity* e )
//{
//
//}
//
//void EntitySystem::onDisabled( Entity* e )
//{
//
//}
//
////From C# 
//void EntitySystem::onChange( Entity* e )
//{
//	bool contains = ( m_systemBits & e->getSystemBits() ) == m_systemBits;
//	bool interest = ( m_componentBits & e->getComponentBits() ) == m_componentBits;
//
//	bitset<SystemType::NUM_SYSTEM_TYPES> empty;
//	
//	if (interest && !contains && m_systemBits != empty) {
//		add(e);
//	} else if (!interest && contains && m_systemBits != empty) {
//		remove(e);
//	} else if ((interest && contains && e->isEnabled()) && m_systemBits != empty) {
//		enable(e);
//	} else if ((interest && contains && e->isEnabled() == false) && m_systemBits != empty) {
//		disable(e);
//	}
//}
//
//void EntitySystem::add( Entity* e )
//{
//	e->addSystemBit(m_systemBits);
//	if (e->isEnabled() == true) {
//		enable(e);
//	}
//	onAdded(e);
//}
//
//void EntitySystem::remove( Entity* e )
//{
//	e->removeSystemBit(m_systemBits);
//	if (e->isEnabled() == true) {
//		disable(e);
//	}
//	onRemoved(e);
//}
//
//void EntitySystem::enable( Entity* e )
//{
//	// If the map contains the index
//	if (m_actives.find(e->getIndex()) != m_actives.end()) {
//		return;
//	}
//	m_actives[e->getIndex()] = e;
//	onEnabled(e);
//}
//
//void EntitySystem::disable( Entity* e )
//{
//	// If the map DOESN'T contain the index
//	if (m_actives.find(e->getIndex()) == m_actives.end()) {
//		return;
//	}
//	m_actives.erase(e->getIndex());
//	onDisabled(e);
//}

void EntitySystem::check(Entity* p_entity) 
{
		if(m_componentBits.none())
		{
			// If this system isn't interested in any components there is no reason to
			// let it continue as it will do nothing.
			return;
		}
		
		bool contains = p_entity->getSystemBits()[m_type.getIndex()];
		bool interested = true; // possibly interested, let's try to prove it wrong.
		
		bitset<ComponentType::NUM_COMPONENT_TYPES> componentBits = 
			p_entity->getComponentBits();

		// Check if the entity possesses ALL of the components defined in the system component bits.
		for ( int i = 0; i < ComponentType::NUM_COMPONENT_TYPES; i++ )
		{
			if ( m_componentBits[i] )
			{
				if ( !componentBits[i] )
				{
					interested = false;
					break;
				}
			}
		}

		if ( interested && !contains )
			insertToSystem( p_entity );
		else if ( !interested && contains )
			removeFromSystem( p_entity );
	}

void EntitySystem::removeFromSystem( Entity* p_entity )
{
	int idx = findEntityInActive( p_entity );
	
	if( idx != -1 )
	{
		p_entity->setSystemBit( m_type.getIndex(), false );
		m_actives.erase( m_actives.begin() + idx );
		removed( p_entity );
		//delete p_entity;
	}
}
void EntitySystem::insertToSystem(Entity* p_entity)
{
	m_actives.push_back( p_entity );
	p_entity->setSystemBit( m_type.getIndex(), true );
	inserted(p_entity);
}

void EntitySystem::added( Entity* p_entity )
{
	check(p_entity);
}

void EntitySystem::changed( Entity* p_entity )
{
	check(p_entity);
}

void EntitySystem::deleted( Entity* p_entity )
{
	if(p_entity->getSystemBits()[m_type.getIndex()])
	{
		removeFromSystem(p_entity);
	}
}

void EntitySystem::enabled( Entity* p_entity )
{
	check(p_entity);
}

void EntitySystem::disabled( Entity* p_entity )
{
	if(p_entity->getSystemBits()[m_type.getIndex()])
	{
		removeFromSystem(p_entity);
	}
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

bitset<ComponentType::NUM_COMPONENT_TYPES> EntitySystem::getComponentBits()
{
	return m_componentBits;
}


int EntitySystem::findEntityInActive( Entity* p_entity )
{
	int idx = -1;

	//NOTE: break in for-loop 
	for( unsigned int i=0; i<m_actives.size(); i++ )
	{
		if( m_actives[i] == p_entity )
		{
			idx = i;
			break;
		}
	}

	return idx;
}

const vector<Entity*>& EntitySystem::getActiveEntities() const
{
	return m_actives;
}