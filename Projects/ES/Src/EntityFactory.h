#pragma once
#include <string>
#include <fstream>
#include <map>
#include "AssemblageHelper.h"
#include "Recipe.h"
#include "EntitySystem.h"

class Entity;
class EntityWorld;
class AssemblageReader;

using namespace std;

// =======================================================================================
//                                      EntityFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief This is a native ES EntitySystem subclass which creates Entities from recipes 
/// which are loaded from assemblage definition files (.asd). 
///        
///	\author		Mattias Liljeson
/// 
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class EntityFactory : public EntitySystem
{
public:
	EntityFactory();
	~EntityFactory();

	///-----------------------------------------------------------------------------------
	/// Read assemblage definition file and create a recipe from it which will be stored
	/// locally and used when queried for an Entity. If a recipe already exists it will be
	/// replaced by this new one.
	/// \param p_filePath Path to the assemblage definition file.
	/// \return AssemblageHelper::E_FileStatus Status of the operation. If the assemblage
	/// was loaded successfully this function will return
	///-----------------------------------------------------------------------------------
	AssemblageHelper::E_FileStatus readAssemblageFile( string p_filePath );

	///-----------------------------------------------------------------------------------
	/// Create an entity by supplying recipe/entity name.
	/// \param p_entityName Name of the entity.
	/// \return Entity* An entity which has been created by the world but not added to it.
	/// This is so that you can make last minute changes to the components and their data
	/// before letting it out in the wild. NULL if the requested recipe isn't found.
	///-----------------------------------------------------------------------------------
	Entity* entityFromRecipe( const string& p_entityName );

private:
	map<string, Recipe*> m_entityRecipes;
};

