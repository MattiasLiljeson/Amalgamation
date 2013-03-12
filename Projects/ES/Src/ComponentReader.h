#pragma once

#include "ComponentData.h"
#include "ComponentType.h"
#include "AssemblageHelper.h"

#include <sstream>
#include <string>
#include <vector>
#include "Ingredient.h"

using namespace std;

class Component;

// =======================================================================================
//                                      ComponentReader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Class used when parsing assemblage definitions to create Ingredients for
/// creating Components. As no one probably will read this I'll leave it at that.
///        
///	\author		Mattias Liljeson
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class ComponentReader
{
public:
	ComponentReader();
	~ComponentReader();

	///-----------------------------------------------------------------------------------
	/// Create an ingredient from a file stream. This will parse the rows with the 
	/// component description and its init data.
	/// \param out_ingredient
	/// \param p_file
	/// \return AssemblageHelper::E_FileStatus
	///-----------------------------------------------------------------------------------
	AssemblageHelper::E_FileStatus parseIngredient( Ingredient** out_ingredient, ifstream* p_file );

private:
	///-----------------------------------------------------------------------------------
	/// Turn one line into a ComponentData.
	/// \param out_data
	/// \param p_dataLine
	/// \return AssemblageHelper::E_FileStatus
	///-----------------------------------------------------------------------------------
	AssemblageHelper::E_FileStatus parseComponentDataLine( ComponentData* out_data, const string& p_dataLine );

private:
	vector<ComponentData> m_componentDataList;
};

