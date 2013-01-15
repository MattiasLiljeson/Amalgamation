#include "Recipe.h"

#include "Entity.h"

Recipe::Recipe( string p_name )
{
	m_name = p_name;
}


Recipe::~Recipe()
{
	for( unsigned int i=0; i<m_ingredients.size(); i++ )
	{
		delete m_ingredients[i];
		m_ingredients[i] = NULL;
	}
}

void Recipe::addIngredient( Ingredient* p_ingredient )
{
	m_ingredients.push_back( p_ingredient );
}

std::string Recipe::getName()
{
	return m_name;
}

void Recipe::cook( Entity* p_entity )
{
	for( unsigned int i=0; i<m_ingredients.size(); i++ )
	{
		Component* component = m_ingredients[i]->prepare();
		if( component != NULL)
		{
			p_entity->addComponent( component->getType(), component );
		}
	}
}
