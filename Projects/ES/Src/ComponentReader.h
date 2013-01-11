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

class ComponentReader
{
public:
	ComponentReader();
	~ComponentReader();

	AssemblageHelper::E_FileStatus parseIngredient( Ingredient** out_ingredient, ifstream* p_file );

private:
	AssemblageHelper::E_FileStatus parseComponentDataLine( ComponentData* out_data, const string& p_dataLine );

private:
	vector<ComponentData> m_componentDataList;
};

