#include "ComponentData.h"

ComponentData::ComponentData()
{
	data = NULL;
	release();
}

void ComponentData::release()
{
	dataType = -1;
	dataName = "";
	dataSize = -1;

	delete data;
	data = NULL;
}

AssemblageHelper::E_FileStatus ComponentData::setDataAsString( stringstream* p_ss )
{
	//Special case for strings
	char dataType;
	string dataName;
	(*p_ss)>>dataType;
	(*p_ss)>>dataName;

	string dataString = "";
	string tempString = "";

	while( p_ss->good() )
	{
		(*p_ss)>>tempString;
		if(dataString != "")
		{
			dataString += " ";
		}
		dataString += tempString; 
	}

	setDataAsCharArray( dataType, dataName,dataString.c_str(), dataString.length()+1 );
	return AssemblageHelper::FileStatus_OK;
}

AssemblageHelper::E_FileStatus ComponentData::getDataAsString( string* out_data )
{
	char* cStr;
	getDataAsCharArray(&cStr);

	if(out_data != NULL )
	{
		*out_data = string(cStr);
		return AssemblageHelper::FileStatus_OK;
	}
	else
	{
		return AssemblageHelper::FileStatus_COMPONENT_DATA_CONVERSION_FAILED;
	}
}

void ComponentData::setDataAsCharArray( char p_dataType,
		string p_dataName, const char* p_src, int p_stringLength )
{
	release();

	dataType = p_dataType;
	dataName = p_dataName;

	// void* = 4 bytes. char = 1 byte.
	dataSize = ceil( p_stringLength / (float)4 );
	data = new void*[dataSize];
	memcpy( data, p_src, dataSize * sizeof(void*) );
}

void ComponentData::getDataAsCharArray( char** out_data )
{
	*out_data = reinterpret_cast<char*>( data );
}