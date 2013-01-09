#include "ComponentReader.h"
#include "ComponentFactory.h"

#include "Component.h"
#include "ComponentData.h"

ComponentReader::ComponentReader(void)
{
}


ComponentReader::~ComponentReader(void)
{
	for( unsigned int i=0; i<m_componentDataList.size(); i++ )
	{
		m_componentDataList[i].release();
	}
}

AssemblageHelper::E_FileStatus ComponentReader::createComponent( 
	ComponentType::ComponentTypeIdx* out_type, Component** out_component, ifstream* p_file )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	Component* component = NULL;

	char componentPrefix = AssemblageHelper::peekCharFromStream( &componentPrefix, p_file );
	// If there's no error and componentName defines a component
	if( status == AssemblageHelper::FileStatus_OK && componentPrefix == 'c' )
	{
		string componentName ="";
		status = AssemblageHelper::readLineFromStream( &componentPrefix, &componentName, p_file );
		
		// Create correct component
		component = ComponentFactory::createInstance( componentName );
		if( component == NULL)
		{
			status = AssemblageHelper::FileStatus_COMPONENT_NOT_FOUND;
		}
		else
		{
			// As long as the file is ok and the next line defines data. Create Componentdata
			char nextPrefix = '\0';
			status = AssemblageHelper::peekCharFromStream( &nextPrefix, p_file );
			while( status == AssemblageHelper::FileStatus_OK && nextPrefix == 'd')
			{
				char dataPrefix = '\0';
				string dataLine = "";

				// read componentName, make sure it's data
				status = AssemblageHelper::readLineFromStream( &dataPrefix, &dataLine, p_file );
				if( dataPrefix == 'd')
				{
					status = readComponentDataLine( dataLine );
				}

				component->init( m_componentDataList );
			}
		}
	}

	if( out_component != NULL )
	{
		*out_component = component;
	}

	return status;
}

AssemblageHelper::E_FileStatus ComponentReader::readComponentDataLine( const string& p_dataLine )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	stringstream ss( p_dataLine );

	ComponentData data;

	switch(data.dataType)
	{
	case 'b':
		data.setData<bool>( &data, &ss );
	case 'i':
		data.setData<int>( &data, &ss );
		break;
	case 'u':
		data.setData<unsigned int>( &data, &ss );
		break;
	case 'f':
		data.setData<float>( &data, &ss );
		break;
	case 'd':
		data.setData<double>( &data, &ss );
		break;
	case 'c':
		data.setData<char>( &data, &ss );
		break;
	case 's':
		data.setData<string>( &data, &ss );
		break;
	default:
		status = AssemblageHelper::FileStatus_COMPONENT_DATA_TYPE_NOT_SUPPORTED;
		break;
	}

	if( status == AssemblageHelper::FileStatus_OK)
	{
		m_componentDataList.push_back( data );
	}

	return status;
}
