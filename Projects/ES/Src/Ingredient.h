
#pragma once

#include <string>
#include <vector>
#include "Component.h"
#include "ComponentFactory.h"

using namespace std;

class Ingredient
{
public:
	Ingredient( string p_name );
	~Ingredient();
	Component* prepare();
	void addData( ComponentData p_data );

private:
	string m_name;
	vector<ComponentData> m_data;
};