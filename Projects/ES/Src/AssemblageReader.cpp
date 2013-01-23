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

AssemblageHelper::E_FileStatus AssemblageReader::readAssemblageFile( Recipe** out_recipe, string p_filePath )
{
	ifstream file( p_filePath, ifstream::in );

	AssemblageHelper::E_FileStatus status = AssemblageHelper::controlStream( &file );
	if( status == AssemblageHelper::FileStatus_OK )
	{
		// File ok, read!
		AssemblageReader reader;
		status = reader.parseFile( out_recipe, &file );
	}

	file.close();

	return status;
}

AssemblageHelper::E_FileStatus AssemblageReader::parseFile( Recipe** out_recipe, ifstream* p_file )
{
	Recipe* recipe = NULL;
	char prefix = ' ';
	string entityName = "";
	AssemblageHelper::E_FileStatus status = AssemblageHelper::readLineFromStream( &prefix, &entityName, p_file );

	if( status == AssemblageHelper::FileStatus_OK )
	{
		if( prefix != 'e' )
		{
			status = AssemblageHelper::FileStatus_WRONG_FILE_FORMAT;
		}

		recipe = new Recipe( entityName );
		status = parseRecipe( recipe, p_file );
	}

	if( status == AssemblageHelper::FileStatus_OK ||
		status == AssemblageHelper::FileStatus_END_OF_FILE )
	{
		*out_recipe = recipe;
	}
	else
	{
		delete recipe;
		recipe = NULL;
	}

	return status;
}

AssemblageHelper::E_FileStatus AssemblageReader::parseRecipe( Recipe* out_recipe,
														  ifstream* p_file )
{
	// Read first entityName.
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	char nextPrefix;

	// Read components
	while( status == AssemblageHelper::FileStatus_OK )
	{
		status = AssemblageHelper::peekCharFromStream( &nextPrefix, p_file );
		if( nextPrefix != 'c' )
		{
			// Swallow next line and look for next component
			status = AssemblageHelper::readLineFromStream( NULL, NULL, p_file );
			//status = AssemblageHelper::FileStatus_WRONG_FILE_FORMAT;

		}
		else
		{
			// The next line defines a component
			//Component* component = NULL;
			//ComponentType::ComponentTypeIdx type = ComponentType::NON_EXISTING;
			ComponentReader reader;
			// Read component data
			Ingredient* ingredient = NULL;
			status = reader.parseIngredient( &ingredient, p_file );
			
			//// If the component exists
			//if( type != ComponentType::ComponentTypeIdx::NON_EXISTING )
			//{
			//	out_entity->addComponent( type, component );
			//}

			if( ingredient != NULL )
			{
				if( status == AssemblageHelper::FileStatus_OK ||
					status == AssemblageHelper::FileStatus_END_OF_FILE )

				{
					out_recipe->addIngredient( ingredient );
				}
				else
				{
					delete ingredient;
					ingredient = NULL;
					//status = AssemblageHelper::FileStatus_COMPONENT_TYPE_IDX_NOT_SET;
				}
			}
		}
	}
	return status;
}
