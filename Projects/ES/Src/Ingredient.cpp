#include "Ingredient.h"

Ingredient::Ingredient( string p_name )
{
	m_name = p_name;
}

Ingredient::~Ingredient()
{
	for( unsigned int i=0; i<m_data.size(); i++ )
	{
		m_data[i].release();
	}
}

void Ingredient::addData( ComponentData p_data )
{
	m_data.push_back( p_data );
}

Component* Ingredient::prepare()
{
	Component* comp = ComponentFactory::createInstance( m_name );
	if( comp != NULL )
	{
		comp->init( m_data );
	}
	return comp;
}
