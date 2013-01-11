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

AssemblageHelper::E_FileStatus ComponentReader::parseIngredient( 
	Ingredient** out_ingredient, ifstream* p_file )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	Ingredient* ingredient = NULL;

	char componentPrefix = ' ';
	status = AssemblageHelper::peekCharFromStream( &componentPrefix, p_file );
	// If there's no error and componentName defines a component
	if( status == AssemblageHelper::FileStatus_OK && componentPrefix == 'c' )
	{
		string componentName ="";
		status = AssemblageHelper::readLineFromStream( NULL, &componentName, p_file );

		if( !ComponentFactory::isComponentRegistrered( componentName ))
		{
			status = AssemblageHelper::FileStatus_COMPONENT_NOT_FOUND;
		}
		else
		{
			Component* comp = ComponentFactory::createInstance( componentName );
			if( comp->getType() == ComponentType::NON_EXISTING)
			{
				status = AssemblageHelper::FileStatus_COMPONENT_TYPE_IDX_NOT_SET;
			}
			else
			{
				ingredient = new Ingredient( componentName );

				char nextPrefix = ' ';
				// As long as the file is ok and the next line defines data. Create ComponentData
				do {
					status = AssemblageHelper::peekCharFromStream( &nextPrefix, p_file );
					string dataLine = "";

					if( nextPrefix == 'd')
					{
						// read componentName, make sure it's data
						status = AssemblageHelper::readLineFromStream( NULL, &dataLine, p_file );

						ComponentData data;
						status = parseComponentDataLine( &data, dataLine );

						if( status == AssemblageHelper::FileStatus_OK ||
							status == AssemblageHelper::FileStatus_END_OF_FILE )
						{
							ingredient->addData( data );
						}
					} 
				} while( status == AssemblageHelper::FileStatus_OK && nextPrefix == 'd');
			}

			delete comp;
		}
	}

	if( out_ingredient != NULL)
	{
		*out_ingredient = ingredient;
	}
	else
	{
		delete ingredient;
		ingredient = NULL;
	}

	return status;
}

AssemblageHelper::E_FileStatus ComponentReader::parseComponentDataLine(
	ComponentData* out_data,  const string& p_dataLine )
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
		data.setDataAsString( &ss );
		break;
	default:
		status = AssemblageHelper::FileStatus_COMPONENT_DATA_TYPE_NOT_SUPPORTED;
		break;
	}

	if( status == AssemblageHelper::FileStatus_OK ||
		status == AssemblageHelper::FileStatus_END_OF_FILE )
	{
		*out_data = data;
	}
	else
	{
		data.release();
	}

	return status;
}