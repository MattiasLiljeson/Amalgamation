#pragma once

#include "AssemblageHelper.h"

#include<string>
#include<sstream>
using namespace std;

struct ComponentData
{
	char dataType;
	string dataName;  
	void** data;
	int dataSize;

	ComponentData();
	void release();

	template<typename T>
	void setData( stringstream* p_ss );
	
	template<typename T>
	AssemblageHelper::E_FileStatus getData( T* out_value );
	
	// NOTE: Will never return conversion failed as raw memcpy is used. No type 
	// checking is done! Only returns if the variable wasn't defined.
	AssemblageHelper::E_FileStatus setDataAsString( stringstream* p_ss );
	AssemblageHelper::E_FileStatus getDataAsString( string* out_data );

	void setDataAsCharArray( char p_dataType, string p_dataName, const char* p_src, int p_size );
	void getDataAsCharArray( char** out_data );
};

template<typename T>
void ComponentData::setData( stringstream* p_ss )
{
	// Make sure no memory leaks 
	release();

	// Read line from stream
	T rawData;
	(*p_ss)>>dataType;
	(*p_ss)>>dataName;
	(*p_ss)>>rawData;
	dataSize = sizeof(rawData);

	// Convert data to void* and save it.
	data = new void*[dataSize];
	memcpy( data, &rawData, dataSize );
}

template<typename T>
AssemblageHelper::E_FileStatus ComponentData::getData( T* out_value )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	T* value;
	value = reinterpret_cast<T*>( data );

	if( out_value != NULL)
	{
		*out_value = *value;
		return AssemblageHelper::FileStatus_OK;
	}
	else
	{
		return AssemblageHelper::FileStatus_COMPONENT_DATA_CONVERSION_FAILED;
	}
}