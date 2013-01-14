#pragma once

#include <fstream>
#include <string>

using namespace std;

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
	enum E_AssemblageDataTypes
	{
	};

public:
	AssemblageHelper();
	~AssemblageHelper();

	static E_FileStatus peekCharFromStream( char* out_prefix, ifstream* p_file );
	static E_FileStatus readLineFromStream( char* out_prefix, string* out_line,
		ifstream* p_file );
	static bool skipLine( char p_prefix );
	static bool skipChar( char p_prefix );
	static E_FileStatus controlStream( ifstream* p_file );
};

