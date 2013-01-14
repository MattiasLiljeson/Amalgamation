#pragma once

class Component;

#include <map>

using namespace std;

// Template function used when mapping string to classes. 
template<typename T> Component * createT() { return new T; }

// =======================================================================================
//                                      ComponentFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief		Factory for creating components from their registered name.
/// \author		Mattias Liljeson 
/// \Warning	Make sure to register all components you wish to be able to create with
/// this factory. Info on how to do that can be found in the wiki. 
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class ComponentFactory
{
public:
	typedef std::map<std::string, Component*(*)()> map_type;

	ComponentFactory();
	~ComponentFactory();


	///-----------------------------------------------------------------------------------
	/// Function used to find out if a component has been registered together with a
	/// creation function.
	/// \param p_componentName Name of the component.
	/// \return bool True if the component has been registered. False otherwise
	///-----------------------------------------------------------------------------------
	static bool isComponentRegistrered( std::string& p_componentName );


	///-----------------------------------------------------------------------------------
	/// Create an instance of a component by using the map of component names and 
	/// component creation functions.
	/// \param p_componentName Name of the component to create.
	/// \return Component* A created component. NULL if the component isn't registered.
	///-----------------------------------------------------------------------------------
	static Component* createInstance( std::string& p_componentName );

protected:

	///-----------------------------------------------------------------------------------
	/// Used to get a pointer to the map used to store component names and component
	/// creation functions. This triggers a static stack allocation of the map if this
	/// function hasn't been called before and returns that i subsequent calls. This will
	/// delete itself when the program terminates.
	/// \return map_type* The map itself
	///-----------------------------------------------------------------------------------
	static map_type* getMap();

private:
	static map_type s_map;
};

#pragma once

// =======================================================================================
//                                      ComponentRegister
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief		A class used by the Component to register itself. A subclass of the 
/// components type must be registered with the factory if you wish to be able to create 
/// the class from the factory.    
/// \author		Mattias Liljeson
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------
template<typename T>
class ComponentRegister : ComponentFactory 
{ 
public:

	///-----------------------------------------------------------------------------------
	/// Constructor used when registering a component with the factory.
	/// \param p_componentName Name of the component to register.
	/// \return 
	///-----------------------------------------------------------------------------------
	ComponentRegister(std::string const& p_componentName)
	{ 
		map_type* map = getMap();
		map->insert(std::make_pair(p_componentName, &createT<T>));
	}
};
