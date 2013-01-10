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
	char nextPrefix;

	// Read components
	while( status == AssemblageHelper::FileStatus_OK )
	{
		status = AssemblageHelper::peekCharFromStream( &nextPrefix, p_file );
		if( nextPrefix != 'c' )
		{
			//swallow next line and look for next component
			status = AssemblageHelper::readLineFromStream( NULL, NULL, p_file );
			//status = AssemblageHelper::FileStatus_WRONG_FILE_FORMAT;

		}
		else
		{
			// The next line defines a component
			Component* component = NULL;
			ComponentType::ComponentTypeIdx type = ComponentType::NON_EXISTING;
			ComponentReader reader;
			// Read component data
			status = reader.createComponent( &type, &component, p_file );
			
			// If the component exists
			if( type != ComponentType::ComponentTypeIdx::NON_EXISTING )
			{
				out_entity->addComponent( type, component );
			}
			else
			{
				delete component;
				component = NULL;
				status = AssemblageHelper::FileStatus_COMPONENT_TYPE_IDX_NOT_SET;
			}
		}
	}
	return status;
}
