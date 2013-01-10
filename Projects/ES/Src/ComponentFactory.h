#pragma once

class Component;

#include <map>

using namespace std;

// Template function used when mapping string to classes. 
template<typename T> Component * createT() { return new T; }

class ComponentFactory
{
public:
	typedef std::map<std::string, Component*(*)()> map_type;

	ComponentFactory();
	~ComponentFactory();

	static Component* createInstance(std::string& p_componentName);

protected:
	static map_type* getMap();

private:
	static map_type s_map; //TODO: Change to stack allocation
};

template<typename T>
class ComponentRegister : ComponentFactory 
{ 
public:
	ComponentRegister(std::string const& s)
	{ 
		map_type* map = getMap();
		map->insert(std::make_pair(s, &createT<T>));
	}
};
