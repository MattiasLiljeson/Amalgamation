#pragma once

#include <fstream>
#include <string>
#include <deque>

using namespace std;

// =======================================================================================
//                                      AssemblageHelper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief		A helper class containing string, char and stream manipulation functions
/// and enums used to return status when calling the above functions. These enums are used
/// in other places where assemblages are worked with
///	\author		Mattias Liljeson
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class AssemblageHelper
{
public:


	enum E_FileStatus
	{
		FileStatus_OK,
		FileStatus_FILE_NOT_FOUND,
		FileStatus_WRONG_FILE_FORMAT,
		FileStatus_FILE_STREAM_CRASHED,
		FileStatus_READING_FILE_FAILED,
		FileStatus_END_OF_FILE,
		// Mapping between string and class not done!
		FileStatus_COMPONENT_NOT_FOUND,
		// The Component has not had it's typeIdx set!
		FileStatus_COMPONENT_TYPE_IDX_NOT_SET,
		// Add data type to ComponentReader if you want to parse it or split it to built
		// in types and reconstruct it in the components init-function.
		FileStatus_COMPONENT_DATA_TYPE_NOT_SUPPORTED,
		// Are you casting to correct classes? have you supplied a non-NULL ptr into which
		// the data will be read?
		FileStatus_COMPONENT_DATA_CONVERSION_FAILED,
		// The variable wasn't given any value
		FileStatus_COMPONENT_DATA_NOT_SPECIFIED,
		FileStatus_ELEMENT_COUNT
	};

	// Not used yet
	enum AssemblageDataTypes
	{
		AssemblageDataTypes_NON_EXISTING = -1,
		AssemblageDataTypes_FIRST,

		AssemblageDataTypes_BOOL = AssemblageDataTypes_FIRST,
		AssemblageDataTypes_INT,
		AssemblageDataTypes_UINT,
		AssemblageDataTypes_FLOAT,
		AssemblageDataTypes_DOUBLE,
		AssemblageDataTypes_CHAR,
		AssemblageDataTypes_STRING,
		AssemblageDataTypes_VEC2,
		AssemblageDataTypes_VEC3,
		AssemblageDataTypes_VEC4,

		AssemblageDataTypes_LAST = AssemblageDataTypes_VEC4, 
		AssemblageDataTypes_CNT
	};

public:
	AssemblageHelper();
	~AssemblageHelper();

	///-----------------------------------------------------------------------------------
	/// Find the next char in the stream which isn't a newline
	/// \param out_prefix The variable which will be set
	/// \param p_file which stream to use
	/// \return AssemblageHelper::E_FileStatus Status of the file stream
	///-----------------------------------------------------------------------------------
	static E_FileStatus peekNextCharFromStream( char* out_prefix, ifstream* p_file );

	///-----------------------------------------------------------------------------------
	/// Returns the next line which is not a newline
	/// \param out_prefix The prefix of the line
	/// \param out_line The rest of the line
	/// \param p_file Stream to read from
	/// \return AssemblageHelper::E_FileStatus
	///-----------------------------------------------------------------------------------
	static E_FileStatus readLineFromStream( char* out_prefix, string* out_line,
		ifstream* p_file );

	///-----------------------------------------------------------------------------------
	/// Helper function used to check whether to skip the next line due to newlines, 
	/// comments etc
	/// \param p_prefix The prefix used to check the line with.
	/// \return bool tru if next line should be skipped
	///-----------------------------------------------------------------------------------
	static bool isLineToSkip( char p_prefix );

	///-----------------------------------------------------------------------------------
	/// Helper function used to check whether to skip the next char due to white spaces
	/// \param p_prefix The prefix to check.
	/// \return bool true if char is to skip
	///-----------------------------------------------------------------------------------
	static bool isCharToSkip( char p_prefix );

	///-----------------------------------------------------------------------------------
	/// Helper function which controls the stream for errors and reports them by with the 
	/// correct enum through the return value 
	/// \param p_file
	/// \return AssemblageHelper::E_FileStatus
	///-----------------------------------------------------------------------------------
	static E_FileStatus controlStream( ifstream* p_file );

	static E_FileStatus peekNextCharFromLine( char* out_prefix, ifstream* p_line );

	static AssemblageDataTypes typeFromString( const string& p_typeAsStr );

	static deque<string> splitString( const string& p_string, char p_denominator );
};

