#include "EntityFactory.h"

#include "AssemblageReader.h"
#include "Entity.h"

EntityFactory::EntityFactory() 
	: EntitySystem( SystemType::EntityFactory ) 
{
}

EntityFactory::~EntityFactory()
{
	map<string, Recipe*>::iterator it;

	for( it = m_entityRecipes.begin(); it != m_entityRecipes.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}
	m_entityRecipes.clear();
}

AssemblageHelper::E_FileStatus EntityFactory::readAssemblageFile( string p_filePath )
{
	Recipe* newRecipe = NULL;
	AssemblageReader reader;
	AssemblageHelper::E_FileStatus status = reader.readAssemblageFile( &newRecipe, p_filePath );

	// The file was parsed correctly and a entity was defined in it
	if( status == AssemblageHelper::FileStatus_OK ||
		status == AssemblageHelper::FileStatus_END_OF_FILE )
	{
		if( newRecipe != NULL )
		{
			// Delete previous recipe with the same name, it existing.
			delete m_entityRecipes[newRecipe->getName()];
			m_entityRecipes[newRecipe->getName()] = newRecipe;
		}
	}
	
	return status;
}

Entity* EntityFactory::entityFromRecipe( const string& p_entityName )
{
	Entity* meal = NULL;

	map<string, Recipe*>::iterator it = m_entityRecipes.find( p_entityName );
	if( it != m_entityRecipes.end())
	{
		meal = m_world->createEntity();
		it->second->cook( meal );
	}

	return meal;
}
