#pragma once

#include "ComponentData.h"
#include "ComponentType.h"
#include "AssemblageHelper.h"

#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Component;

class ComponentReader
{
public:
	ComponentReader();
	~ComponentReader();

	AssemblageHelper::E_FileStatus createComponent( ComponentType::ComponentTypeIdx* out_type,
		Component** out_component, ifstream* p_file );

private:
	AssemblageHelper::E_FileStatus readComponentDataLine( const string& p_dataLine );

private:
	vector<ComponentData> m_componentDataList;
};

