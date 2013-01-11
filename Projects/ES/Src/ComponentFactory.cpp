#include "ComponentFactory.h"

#include "Component.h"
#include <string>

//ComponentFactory::map_type* ComponentFactory::s_map = NULL;
//ComponentFactory::map_type ComponentFactory::s_map;

ComponentFactory::ComponentFactory()
{

}


ComponentFactory::~ComponentFactory()
{
}

Component* ComponentFactory::createInstance( std::string& p_componentName )
{
	map_type* map = getMap();
	map_type::iterator it = map->find( p_componentName );
	if( it == map->end() )
	{
		return 0;
	}
	return it->second();
}

ComponentFactory::map_type* ComponentFactory::getMap()
{
	// never deleted. (exist until program termination)
	// because we can't guarantee correct destruction order 
	//if(!s_map)
	//{ 
	//	s_map = new map_type; 
	//}
	//return s_map;

	static map_type instance;
	return &instance;
}

bool ComponentFactory::isComponentRegistrered( std::string& p_componentName )
{
	map_type* map = getMap();
	map_type::iterator it = map->find( p_componentName );
	if( it == map->end() )
	{
		return false;
	}
	return true;
}
