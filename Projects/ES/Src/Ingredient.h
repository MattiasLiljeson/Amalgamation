
#pragma once

#include <string>
#include <vector>
#include "Component.h"
#include "ComponentFactory.h"

using namespace std;

// =======================================================================================
//                                      Ingredient
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Class describing an ingredient used by recipes when cooking (creating) 
/// entities. 
///        
///	\author		Mattias Liljeson
///
/// # Ingredient
/// An assortment of componentdatas which together with a component name instantiate a 
/// component with init data.
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------
class Ingredient
{
public:
	///-----------------------------------------------------------------------------------
	/// Constructor. 
	/// \param p_name The name of the component to be constructed. Must be mapped in 
	/// ComponentFactory.
	/// \return 
	///-----------------------------------------------------------------------------------
	Ingredient( string p_name );
	~Ingredient();

	///-----------------------------------------------------------------------------------
	/// Create a component with the init data (componentData) that is stored in this
	/// instance.
	/// \return Component* If a component with the name that this instance stores exists,
	/// a newly created component with all componentDatas applied will be replied. 
	/// Otherwise NULL
	///-----------------------------------------------------------------------------------
	Component* prepare();

	///-----------------------------------------------------------------------------------
	/// Add init data to this ingredient
	/// \param p_data The data that will be used
	/// \return void
	///-----------------------------------------------------------------------------------
	void addData( ComponentData p_data );

private:
	string m_name;
	vector<ComponentData> m_data;
};