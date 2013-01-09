#include "AssemblageReader.h"
#include "AssemblageHelper.h"
#include "Component.h"
#include "ComponentReader.h"
#include "Entity.h"
#include "ComponentType.h"

AssemblageReader::AssemblageReader()
{
}


AssemblageReader::~AssemblageReader()
{
}

AssemblageHelper::E_FileStatus AssemblageReader::readFile( Entity* out_entity,
														  ifstream* p_file )
{
	// Read first line.
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	char prefix;
	string componentName = "";
	status = AssemblageHelper::readLineFromStream( &prefix, &componentName, p_file );

	// Read components
	while( status == AssemblageHelper::FileStatus_OK )
	{
		if( status == AssemblageHelper::FileStatus_OK )
		{
			if( prefix != 'c' )
			{
				status = AssemblageHelper::FileStatus_WRONG_FILE_FORMAT;
			}
		}

		// Read component data
		char nextPrefix = '\0';
		status = AssemblageHelper::peekCharFromStream( &nextPrefix, p_file );
		while( status == AssemblageHelper::FileStatus_OK )
		{
			// If the next line defines a component
			if( nextPrefix == 'c' )
			{
				// Read component data
				Component* component = NULL;
				ComponentType::ComponentTypeIdx type;
				ComponentReader reader;
				status = reader.createComponent( &type, &component, p_file );
				out_entity->addComponent( type, component );
			}
			else
			{
				// Read a line and then discard it to read the next line
				AssemblageHelper::readLineFromStream( NULL, NULL, p_file );
			}
			
			// Peek for prefix on the next line
			status = AssemblageHelper::peekCharFromStream( &nextPrefix, p_file );
		}

	}
	return status;
}
