#include "EntityFactory.h"

#include "AssemblageReader.h"
#include "Entity.h"

EntityFactory::EntityFactory()
{
}


EntityFactory::~EntityFactory()
{
}

AssemblageHelper::E_FileStatus EntityFactory::readAssemblageFile( Entity* out_entity, string p_filePath )
{
	ifstream file( p_filePath, ifstream::in );

	AssemblageHelper::E_FileStatus status = AssemblageHelper::controlStream( &file );
	if( status != AssemblageHelper::FileStatus_OK )
	{
		file.close();
	}
	else
	{
		// File ok, read!
		status = parseFile( out_entity, &file );
	}

	return status;
}

void EntityFactory::initAssamblageReaders()
{

}

AssemblageHelper::E_FileStatus EntityFactory::parseFile( Entity* out_entity, ifstream* p_file )
{
	char prefix = ' ';
	string line = "";
	AssemblageHelper::E_FileStatus status = AssemblageHelper::readLineFromStream( &prefix, &line, p_file );

	if( status == AssemblageHelper::FileStatus_OK )
	{
		if( prefix != 'e' )
		{
			status = AssemblageHelper::FileStatus_WRONG_FILE_FORMAT;
		}
	}
	
	// Create Entity
	AssemblageReader reader;
	if( status == AssemblageHelper::FileStatus_OK )
	{
		status = reader.readFile( out_entity, p_file );
	}

	return status;
}