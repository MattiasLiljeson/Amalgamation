#include "ComponentReader.h"
#include "ComponentFactory.h"

#include "Component.h"
#include "ComponentData.h"
#include "AssemblageException.h"

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
	status = AssemblageHelper::peekNextCharFromStream( &componentPrefix, p_file );
	// If there's no error and componentName defines a component
	if( status == AssemblageHelper::FileStatus_OK && componentPrefix == 'c' )
	{
		string componentName ="";
		status = AssemblageHelper::readLineFromStream( NULL, &componentName, p_file );

		if( !ComponentFactory::isComponentRegistrered( componentName ))
		{
			status = AssemblageHelper::FileStatus_COMPONENT_NOT_FOUND;
			throw AssemblageException("Component "+componentName+" not found! ",
										__FILE__,__FUNCTION__,__LINE__);
		}
		else
		{
			Component* comp = ComponentFactory::createInstance( componentName );
			if( comp->getComponentTypeId() == ComponentType::NON_EXISTING)
			{
				status = AssemblageHelper::FileStatus_COMPONENT_TYPE_IDX_NOT_SET;
			}
			else
			{
				ingredient = new Ingredient( componentName );

				char nextPrefix = ' ';
				// As long as the file is ok and the next line defines data. Create ComponentData
				do {
					status = AssemblageHelper::peekNextCharFromStream( &nextPrefix, p_file );
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

	deque<string> rowElements = AssemblageHelper::splitString( p_dataLine, ',' );
	int type = AssemblageHelper::typeFromString( rowElements[0] );

	ComponentData data;
	data.setData( rowElements );
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