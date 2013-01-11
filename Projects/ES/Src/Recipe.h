#pragma once

#include <map>
#include <string>
#include <vector>
#include "ComponentData.h"
#include <utility>
#include "Ingredient.h"

class Entity;

using namespace std;

class Recipe
{
public:
	Recipe( string p_name );
	~Recipe();

	void addIngredient( Ingredient* p_ingredient );
	string getName();
	void cook( Entity* p_entity );

private:
	string m_name;
	vector<Ingredient*> m_ingredients;
};

