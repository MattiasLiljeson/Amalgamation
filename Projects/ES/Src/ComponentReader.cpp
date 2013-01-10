#include "ComponentReader.h"
#include "ComponentFactory.h"

#include "Component.h"
#include "ComponentData.h"

ComponentReader::ComponentReader()
{
}


ComponentReader::~ComponentReader()
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

	char componentPrefix = ' ';
	status = AssemblageHelper::peekCharFromStream( &componentPrefix, p_file );
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
			char nextPrefix = ' ';
			status = AssemblageHelper::peekCharFromStream( &nextPrefix, p_file );
			while( status == AssemblageHelper::FileStatus_OK && nextPrefix == 'd')
			{
				char dataPrefix = ' ';
				string dataLine = "";

				// read componentName, make sure it's data
				status = AssemblageHelper::readLineFromStream( &dataPrefix, &dataLine, p_file );
				if( dataPrefix == 'd')
				{
					status = readComponentDataLine( dataLine );
				}
			}
			component->init( m_componentDataList );
		}
	}

	ComponentType::ComponentTypeIdx type = ComponentType::NON_EXISTING;
	if( component != NULL )
	{
		type = component->getType();
	}

	if( out_type != NULL)
	{
		*out_type = type;
	}

	if( out_component != NULL)
	{
		*out_component = component;
	}
	else
	{
		delete component;
		component = NULL;
	}

	return status;
}

AssemblageHelper::E_FileStatus ComponentReader::readComponentDataLine( const string& p_dataLine )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	stringstream ss( p_dataLine );

	char dataType = ss.peek();

	ComponentData data;

	switch( dataType )
	{
	case 'b':
		data.setData<bool>( &ss );
	case 'i':
		data.setData<int>( &ss );
		break;
	case 'u':
		data.setData<unsigned int>( &ss );
		break;
	case 'f':
		data.setData<float>( &ss );
		break;
	case 'd':
		data.setData<double>( &ss );
		break;
	case 'c':
		data.setData<char>( &ss );
		break;
	case 's':
		{
			//Special case for strings
			char dataType;
			string dataName;
			ss>>dataType;
			ss>>dataName;

			string dataString = "";
			string tempString = "";

			while( ss.good() )
			{
				ss>>tempString;
				if(dataString != "")
				{
					dataString += " ";
				}
				dataString += tempString; 
			}

			data.setDataAsCharArray( dataType, dataName,
				dataString.c_str(), dataString.length()+1 );
		}
		break;
	default:
		status = AssemblageHelper::FileStatus_COMPONENT_DATA_TYPE_NOT_SUPPORTED;
		break;
	}

	if( status == AssemblageHelper::FileStatus_OK )
	{
		m_componentDataList.push_back( data );
	}

	return status;
}
