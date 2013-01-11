#include "FileCheck.h"
#include "ToString.h"


bool isFileOk( const string& filepath, 
			   string& p_outMsg,
			   const string &p_callerCodeFile, 
			   const string &p_callerCodeFunc, 
			   int p_callerCodeLine )
{
	ifstream file(filepath);
	bool res = file.good();
	if (!res)
	{
		p_outMsg = string("Error opening file '")+filepath+"' in "+p_callerCodeFile+" | "+p_callerCodeFunc+" line "+toString(p_callerCodeLine);
	}
	else
	{
		// Not really needed: p_outMsg = string("File '")+filepath+"' is OK!";
	}
	return res;
}
