#include "AssemblageHelper.h"

AssemblageHelper::AssemblageHelper(void)
{
}


AssemblageHelper::~AssemblageHelper(void)
{
}

AssemblageHelper::E_FileStatus AssemblageHelper::peekCharFromStream( char* out_prefix,
																	ifstream* p_file )
{
	char nextPrefix = '\0';
	E_FileStatus status = controlStream( p_file);
	if( status == FileStatus_OK )
	{
		nextPrefix = p_file->peek();
		while( status == FileStatus_OK && skipLine( nextPrefix ) )
		{
			string discard = "";
			getline( *p_file, discard );
			nextPrefix = p_file->peek();
			status = controlStream( p_file );
		};
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
			} while( skipChar(prefix) );
		} while( skipLine(prefix) && p_file->good() );
	}
	
	string subLine = "";
	if( status == FileStatus_OK )
	{
		// prefix + 2 is the first character with data.
		subLine = line.substr( prefixPos+2, string::npos );
	}

	if( out_line != NULL )
	{
		*out_line = subLine;
	}
	if( out_prefix != NULL )
	{
		*out_prefix = prefix;
	}

	// Check for stream failure
	status = controlStream( p_file );
	return status;
}


bool AssemblageHelper::skipLine( char p_prefix )
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

bool AssemblageHelper::skipChar( char p_prefix )
{
	bool skip = false;

	// Warning: multiple fall-throughs
	// Add comments for whitespace and other skipable chars here
	switch( p_prefix )
	{
	case '\t':
	case ' ':
	
	// Should probably not be here as tehre is a risk of reading outside of a variable
	//case '\0':
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

	if( !p_file->is_open() )
	{
		status = FileStatus_FILE_NOT_FOUND;
	}
	else if( p_file->bad() )
	{
		status = FileStatus_FILE_STREAM_CRASHED; 
	}
	else if( p_file->fail() )
	{
		status = FileStatus_READING_FILE_FAILED;
	}
	else if( p_file->eof() )
	{
		status = FileStatus_END_OF_FILE;
	}

	return status;
}
