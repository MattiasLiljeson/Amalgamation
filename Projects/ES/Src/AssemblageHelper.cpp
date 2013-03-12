#include "AssemblageHelper.h"
#include <algorithm>

AssemblageHelper::AssemblageHelper(void)
{
}


AssemblageHelper::~AssemblageHelper(void)
{
}

AssemblageHelper::E_FileStatus AssemblageHelper::peekNextCharFromStream( char* out_prefix,
																	ifstream* p_file )
{
	char nextPrefix = ' ';
	E_FileStatus status = controlStream( p_file );
	if( status == FileStatus_OK )
	{
		status = peekNextCharFromLine( &nextPrefix, p_file );
		while( status == FileStatus_OK && isLineToSkip( nextPrefix ) )
		{
			string discard = "";
			getline( *p_file, discard );
			status = peekNextCharFromLine( &nextPrefix, p_file );
		}
	}
	
	if( out_prefix != NULL )
	{
		*out_prefix = nextPrefix;
	}
	
	return status;
}

AssemblageHelper::E_FileStatus AssemblageHelper::readLineFromStream( char* out_prefix,
													string* out_line, ifstream* p_file )
{
	int prefixPos = 0;
	char prefix = ' ';
	string line = "";
	
	// Make sure stream is good to go before trying to work with it
	E_FileStatus status = controlStream( p_file);
	if( status == FileStatus_OK )
	{
		// Clear empty lines and comments and then keep the first good line
		do{
			getline( *p_file, line );
		
			prefixPos = -1;
			do {
				prefixPos++;
				prefix = line[prefixPos];
			} while( isCharToSkip( prefix ) );
		} while( isLineToSkip( prefix ) && p_file->good() );
	}
	
	string subLine = "";
	if( status == FileStatus_OK )
	{
		// prefix + 2 is the first character with data.
		subLine = line.substr( prefixPos+2, string::npos );
	}

	if( out_line != NULL ) {
		*out_line = subLine;
	}

	if( out_prefix != NULL ) {
		*out_prefix = prefix;
	}

	// Check for stream failure
	status = controlStream( p_file );
	return status;
}


bool AssemblageHelper::isLineToSkip( char p_prefix )
{
	bool skip = false;

	// Warning: multiple fall-throughs
	// Add chars for comments, empty lines etc here
	switch( p_prefix )
	{
	case '/':
	case '*':
	case '#':
	case '\n':
	case '\r':
	case '\0':
		skip = true;
		break;
	default:
		skip = false;
		break;
	}

	return skip;
}

bool AssemblageHelper::isCharToSkip( char p_prefix )
{
	bool skip = false;

	// Warning: multiple fall-throughs
	// Add comments for whitespace and other skip-able chars here
	switch( p_prefix )
	{
	case '\t':
	case ' ':
	case '\"':
		skip = true;
		break;
	default:
		skip = false;
		break;
	}

	return skip;
}

AssemblageHelper::E_FileStatus AssemblageHelper::controlStream( ifstream* p_file )
{
	E_FileStatus status = FileStatus_OK;

	if( !p_file->is_open() ) {
		status = FileStatus_FILE_NOT_FOUND;
	} else if( p_file->eof() ) {
		status = FileStatus_END_OF_FILE;
	} else if( p_file->bad() ) {
		status = FileStatus_FILE_STREAM_CRASHED; 
	} else if( p_file->fail() ) {
		status = FileStatus_READING_FILE_FAILED;
	}

	return status;
}

AssemblageHelper::E_FileStatus AssemblageHelper::peekNextCharFromLine( char* out_prefix, ifstream* p_line )
{
	char nextPrefix = ' ';
	E_FileStatus status = controlStream( p_line );
	if( status == FileStatus_OK )
	{
		do {
			p_line->get( nextPrefix );
			status = controlStream( p_line );
		} while( status == FileStatus_OK && isCharToSkip( nextPrefix ) );
	}
	p_line->putback( nextPrefix );

	if( out_prefix != NULL ) {
		*out_prefix = nextPrefix;
	}

	return status;
}


AssemblageHelper::AssemblageDataTypes AssemblageHelper::typeFromString( const string& p_typeAsStr )
{
	string typeAsStr;
	typeAsStr.resize( p_typeAsStr.size() );
	std::transform( p_typeAsStr.begin(), p_typeAsStr.end(), typeAsStr.begin(), ::tolower );

	if( typeAsStr == "b" || typeAsStr == "bool" ) {
		return AssemblageDataTypes_BOOL;
	} else if( typeAsStr == "i" || typeAsStr == "int" ) {
		return AssemblageDataTypes_INT;
	} else if( typeAsStr == "u" || typeAsStr == "unsigned" || typeAsStr == "unsignedint" ) {
		return AssemblageDataTypes_UINT;
	} else if( typeAsStr == "f" || typeAsStr == "float" ) {
		return AssemblageDataTypes_FLOAT;
	} else if( typeAsStr == "d" || typeAsStr == "double" ) {
		return AssemblageDataTypes_DOUBLE;
	} else if( typeAsStr == "c" || typeAsStr == "char" ) {
		return AssemblageDataTypes_CHAR;
	} else if( typeAsStr == "s" || typeAsStr == "string" ) {
		return AssemblageDataTypes_STRING;
	} else if( typeAsStr == "vec2" || typeAsStr == "vector2" || typeAsStr == "float2" ) {
		return AssemblageDataTypes_VEC2;
	} else if( typeAsStr == "vec3" || typeAsStr == "vector3" || typeAsStr == "float3" ) {
		return AssemblageDataTypes_VEC3;
	} else if( typeAsStr == "vec4" || typeAsStr == "vector4" || typeAsStr == "float5" ) {
		return AssemblageDataTypes_VEC4;
	} else {
		return AssemblageDataTypes_NON_EXISTING;
	}
}

deque<string> AssemblageHelper::splitString( const string& p_string, char p_denominator )
{
	deque<string> splitted;
	string buf = "";

	int length = p_string.length();
	for( int i=0; i<length; i++ )
	{
		if( p_string[i] != p_denominator && !isCharToSkip( p_string[i] ) ) {
			buf += p_string[i];
		} else if( buf != "" ) {
			splitted.push_back( buf );
			buf = "";
		} else {
			// skip the char if there a more denominators in a row
		}
	}

	// If there's a last word, push it to the deck
	if( buf != "" ) {
		splitted.push_back( buf );
	}

	return splitted;
}

