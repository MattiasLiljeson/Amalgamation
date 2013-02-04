#pragma once
#include <string>
#include <utility>

using namespace std;
class AglVector3;

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
		POINTLIGHT,
		DIRLIGHT,
		SPOTLIGHT
	};

	///-----------------------------------------------------------------------------------
	/// \brief	Super simple solution for data retrieval from empty, 
	/// contains several specifics (for each empty type) as well as generics.
	///-----------------------------------------------------------------------------------
	struct Data
	{
		Data() {name=""; instanceSpecFilename=""; spawnSpecName=""; lightSpecRange=0.0f;
		lightSpecPwr=0.0f; lightSpecGloss=0.0f;}
		string name;
		string instanceSpecFilename;				///< For instance points (file name)
		string spawnSpecName;						///< For spawn points (action name)
		// Light specifics
		// colours
		AglVector3 lightSpecDiffuse, lightSpecSpecular, lightSpecAmbient;
		float lightSpecGloss;
		float lightSpecRange,lightSpecPwr; // range and power
		AglVector3 lightSpecAtt; // attenuation
	};

	static pair<Data,Token> parse(const string& p_string);
protected:
private:
	static string getInstruction(const string& p_string);
	static string extractPart(const string& p_string,int offset=0);
	static float getFloatFromDecimalString(const string& p_string);
	static void getRGB(AglVector3& p_outVec, const string& p_hex);
	static string separators;
	static string decimalSeparator;
	static string instantiate;
	static string connectionpoint;
	static string spawnpoint;
	static string light;
	static string pointlightType;
	static string spotlightType;
	static string dirlightType;
};