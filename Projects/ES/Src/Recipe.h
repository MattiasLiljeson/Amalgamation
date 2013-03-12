#pragma once

#include <map>
#include <string>
#include <vector>
#include "ComponentData.h"
#include <utility>
#include "Ingredient.h"

class Entity;

using namespace std;

// =======================================================================================
//                                      Recipe
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Class describing the creation of an entity.
///        
/// # Recipe
/// Stores the name of the entity and its ingredients. With this information a entity with
/// the correct components and component init data can be constructed.
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------
class Recipe
{
public:
	///-----------------------------------------------------------------------------------
	/// Constructor. 
	/// \param p_name Name of the entity to be constructed with this recipe.
	/// \return 
	///-----------------------------------------------------------------------------------
	Recipe( string p_name );
	~Recipe();


	///-----------------------------------------------------------------------------------
	/// Add ingredient (components description and init data) to the recipe. This class
	/// takes ownership of the object supplied and will delete it in it's constructor.
	/// \param p_ingredient The ingredient which will be added.
	/// \return void
	///-----------------------------------------------------------------------------------
	void addIngredient( Ingredient* p_ingredient );

	///-----------------------------------------------------------------------------------
	/// Recipe/constructed entity name getter
	/// \return std::string name of the recipe/constructed entity
	///-----------------------------------------------------------------------------------
	string getName();


	///-----------------------------------------------------------------------------------
	/// Cook an entity aka Outfit an Entity with components and components init data. The
	/// entity is not registered with a EntityWorld here. Do that before calling this 
	/// function preferably.
	/// \param p_entity The Entity which will be outfitted with the components specified 
	/// in this instance
	/// \return void
	///-----------------------------------------------------------------------------------
	void cook( Entity* p_entity );

private:
	string m_name;
	vector<Ingredient*> m_ingredients;
};

