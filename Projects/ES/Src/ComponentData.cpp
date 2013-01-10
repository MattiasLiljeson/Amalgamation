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

AssemblageHelper::E_FileStatus ComponentData::setDataAsCharArray( char p_dataType,
		string p_dataName, const char* p_src, int p_stringLength )
{
	release();

	dataType = p_dataType;
	dataName = p_dataName;

	// void* = 4 bytes. char = 1 byte.
	dataSize = ceil( p_stringLength / (float)4 );
	data = new void*[dataSize];
	memcpy( data, p_src, dataSize * sizeof(void*) );
	return AssemblageHelper::FileStatus_OK;
}

AssemblageHelper::E_FileStatus ComponentData::getDataAsCharArray( char** out_src )
{
	*out_src = reinterpret_cast<char*>( data );
	return AssemblageHelper::FileStatus_OK;
}