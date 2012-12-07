#ifndef STRING_SPLIT_H
#define STRING_SPLIT_H

#include <string>
#include <queue>
using namespace std;

void stringSplit( string p_line, string p_delimiter, queue< string >& out_splits );
void stringSplitNullTerminated( char* p_line, unsigned int p_byteLength,
							   queue< string >& out_splits );
string getLineToken(const string& line);

#endif