#include "StringSplit.h"

void stringSplit( string p_line, string p_delimiter, queue< string >& out_splits )
{
	int startPosition = 0;
	int endPosition = 0;
	int maxLength = p_line.length();

	while(startPosition >= 0 && startPosition < maxLength)
	{
		endPosition = p_line.find(p_delimiter, startPosition);
		if(endPosition != -1)
		{
			if(startPosition < endPosition)
			{
				string sub = p_line.substr(startPosition, endPosition - startPosition);
				out_splits.push( sub );
			}
			startPosition = endPosition + 1;
		}
		else
		{
			// push the last chunk.
			if(startPosition < maxLength)
			{
				string sub = p_line.substr(startPosition);
				out_splits.push( sub );
				startPosition = maxLength;
			}
		}
	}
}

void stringSplitNullTerminated( char* p_line, unsigned int p_byteLength,
							   queue< string >& out_splits )
{
	unsigned int increase = 0;
	char* p_seekPosition = p_line;

	do
	{
		out_splits.push( string( p_seekPosition ) );
		increase += out_splits.back().size() +1;
		p_seekPosition = p_line + increase;
	} while( increase < p_byteLength );
}



string getLineToken(const string& line)
{
	unsigned int position = 0;
	position = line.find(' ', 0);

	string token = line.substr(0, position);

	return token;
}