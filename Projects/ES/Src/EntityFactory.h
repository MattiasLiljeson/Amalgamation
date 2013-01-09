#pragma once
#include <string>
#include <fstream>
#include <map>
#include "AssemblageHelper.h"

class Entity;
class AssemblageReader;

using namespace std;

class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();

	AssemblageHelper::E_FileStatus readAssemblageFile( Entity* out_entity,
		string p_filePath );

private:
	void initAssamblageReaders();
	AssemblageHelper::E_FileStatus parseFile( Entity* out_entity, ifstream* p_file );
};

