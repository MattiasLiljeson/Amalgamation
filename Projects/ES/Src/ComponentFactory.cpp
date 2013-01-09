#include "ComponentFactory.h"

#include "Component.h"
#include <string>

ComponentFactory::map_type* ComponentFactory::map = new map_type;

ComponentFactory::ComponentFactory()
{
}


ComponentFactory::~ComponentFactory()
{
}

Component* ComponentFactory::createInstance( std::string& p_componentName )
{
	map_type::iterator it = getMap()->find(p_componentName);
	if(it == getMap()->end())
		return 0;
	return it->second();
}

ComponentFactory::map_type* ComponentFactory::getMap()
{
	// never deleted. (exist until program termination)
	// because we can't guarantee correct destruction order 
	if(!map) { map = new map_type; }
	return map; 
}
