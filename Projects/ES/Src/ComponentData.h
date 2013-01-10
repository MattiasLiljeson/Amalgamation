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
	AssemblageHelper::E_FileStatus setDataAsCharArray( char p_dataType, string p_dataName,
		const char* p_src, int p_size );
	AssemblageHelper::E_FileStatus getDataAsCharArray( char** out_src );
	
	template<typename T>
	AssemblageHelper::E_FileStatus setData( stringstream* p_ss );
	template<typename T>
	AssemblageHelper::E_FileStatus getData( T* out_value );

};

// HACK: Will never return conversion failed as raw memcpy is used. No type 
// checking is done!
template<typename T>
AssemblageHelper::E_FileStatus ComponentData::setData( stringstream* p_ss )
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

	return AssemblageHelper::FileStatus_OK;
}

// HACK: Will never return conversion failed as reinterpret cast is used. No type 
// checking is done!
template<typename T>
AssemblageHelper::E_FileStatus ComponentData::getData( T* out_value )
{
	AssemblageHelper::E_FileStatus status = AssemblageHelper::FileStatus_OK;
	T* value;
	if( true )
	{
		value = reinterpret_cast<T*>( data );
		status = AssemblageHelper::FileStatus_OK;
	}

	*out_value = *value;

	return status;
}