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

class EntityFactory : public EntitySystem
{
public:
	EntityFactory();
	~EntityFactory();

	AssemblageHelper::E_FileStatus readAssemblageFile( string p_filePath );

	Entity* entityFromRecipe( const string& p_entityName );

private:
	map<string, Recipe*> m_entityRecipes;
};

