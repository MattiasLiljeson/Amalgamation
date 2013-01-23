#pragma once
#include <string>
#include <utility>

using namespace std;

// =======================================================================================
//                                      MeshNameScriptParser
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # MeshNameScriptParser
/// Detailed description.....
/// Created on: 16-1-2013 
///---------------------------------------------------------------------------------------

class MeshNameScriptParser
{
public:
	enum Token
	{
		MESH,
		CONNECTIONPOINT,
		INSTANTIATE,
		SPAWNPOINT
	};

	struct Data
	{
		string name;
		string filename;
	};

	static pair<Data,Token> parse(const string& p_string);
protected:
private:
	static string getInstruction(const string& p_string);
	static string extractPart(const string& p_string,int offset=0);
	static string separators;
	static string instantiate;
	static string connectionpoint;
	static string spawnpoint;
};