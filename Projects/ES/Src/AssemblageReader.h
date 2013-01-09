#pragma once

#include "EntityFactory.h"

class ComponentReader;
class Entity;

class AssemblageReader
{
public:
	AssemblageReader();
	virtual ~AssemblageReader();

	AssemblageHelper::E_FileStatus readFile( Entity* out_entity, ifstream* p_file );
};

