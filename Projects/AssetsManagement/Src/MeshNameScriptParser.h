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
		SPAWNPOINT,
		LIGHT
	};

	///-----------------------------------------------------------------------------------
	/// \brief	Super simple solution for data retrieval from empty, 
	/// contains several specifics (for each empty type) as well as generics.
	///-----------------------------------------------------------------------------------
	struct Data
	{
		Data() {name=""; instanceSpecFilename=""; spawnSpecName=""; lightSpecR=0.0f;
				lightSpecG=0.0f;; lightSpecB=0.0f; lightSpecRange=0.0f;}
		string name;
		string instanceSpecFilename;				///< For instance points (file name)
		string spawnSpecName;						///< For spawn points (action name)
		// Light specifics
		float lightSpecR, lightSpecG, lightSpecB;
		float lightSpecRange;
		// float lightSpecConst, lightSpecLin, lightSpec work in progress
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
	static string light;
};