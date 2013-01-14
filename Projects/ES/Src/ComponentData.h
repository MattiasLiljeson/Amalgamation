#pragma once

#include "AssemblageHelper.h"

#include<string>
#include<sstream>
using namespace std;

// =======================================================================================
//                                      ComponentData
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Data container struct. Used to transmit data between assemblage files and 
/// Constructed components.
///
///	\author		Mattias Liljeson
///
/// # ComponentData
/// This struct is used to transfer data from Assemblages to the actual constructed 
/// components. This data is stored in avoid pointer array so that all possible data can
/// be stored. Out of the box, support for basic data types and strings are supported
/// where strings need to be handled through a special case due to their internal 
/// structure. Feel free to add support for any datatype you'll like.
///
/// There's no destructor as that would cause a lot of heap allocation and copy operations
/// to occur as you then would need to have a copy constructor. Use release() when you're 
/// finished working with the data instead.
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------
struct ComponentData
{
	char dataType;
	string dataName;  
	void** data;
	int dataSize;

	ComponentData();
	void release();

	///-----------------------------------------------------------------------------------
	/// Setter for basic data types. No error checking for faulty lines etc done. The type
	/// supplied must be allocated on the stack as memcpy is used to save the data.
	/// \param p_ss The stream used to parse type, name and value.
	/// \return void
	///-----------------------------------------------------------------------------------
	template<typename T>
	void setData( stringstream* p_ss );
	
	///-----------------------------------------------------------------------------------
	/// Get the data as the supplied type. Does no type checking whatsoever as 
	/// reinterpret_cast is used.
	/// \param the pointer that will be dereferenced and set to the read data.
	/// \return AssemblageHelper::E_FileStatus If the out parameter is null,
	/// FileStatus_COMPONENT_DATA_CONVERSION_FAILED will be returned. Otherwise
	/// FileStatus_OK will be returned
	///-----------------------------------------------------------------------------------
	template<typename T>
	AssemblageHelper::E_FileStatus getData( T* out_value );
	
	///-----------------------------------------------------------------------------------
	/// Special case function used in conjunction with strings as they store characters on
	/// the heap. Can handle multi word strings.
	///
	/// NOTE: Will never return conversion failed as raw memcpy is used. No type 
	/// checking is done! 
	/// \param p_ss The stream used to parse type, name and value.
	/// \return AssemblageHelper::E_FileStatus returns FileStatus_OK if everything is 
	/// fine. Returns FileStatus_COMPONENT_DATA_CONVERSION_FAILED if the variable wasn't 
	///defined in the assemblage.
	///-----------------------------------------------------------------------------------
	AssemblageHelper::E_FileStatus setDataAsString( stringstream* p_ss );


	///-----------------------------------------------------------------------------------
	/// Get the data as a string. Does no type checking whatsoever as reinterpret_cast is
	/// used.
	/// \param out_data The pointer that will be dereferenced and set to the read data.
	/// \return AssemblageHelper::E_FileStatus If the out parameter is null, 
	/// FileStatus_COMPONENT_DATA_CONVERSION_FAILED will be returned. Otherwise
	/// FileStatus_OK will be returned
	///-----------------------------------------------------------------------------------
	AssemblageHelper::E_FileStatus getDataAsString( string* out_data );

	///-----------------------------------------------------------------------------------
	/// Manually set contents of this struct as char array.
	/// \param p_dataType A char the symbolizes the data type. Should be 'a' as in array. 
	/// Use other char if you store other information than chars or if you want to parse
	/// the contents in a special way
	/// \param p_dataName Name of the data/variable
	/// \param p_src Where to start copying from
	/// \param p_size size of c string / number of bytes to copy.
	/// \return void
	///-----------------------------------------------------------------------------------
	void setDataAsCharArray( char p_dataType, string p_dataName, const char* p_src, int p_size );

	///-----------------------------------------------------------------------------------
	/// Fetch data as a c string.
	/// \param pointer to char pointer which will be set to point to the internal data of
	/// this instance. Will be destroyed when release() is called. 
	/// \return void
	///-----------------------------------------------------------------------------------
	void getDataAsCharArray( char** out_data);
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