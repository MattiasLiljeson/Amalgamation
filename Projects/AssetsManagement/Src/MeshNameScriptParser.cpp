#include "MeshNameScriptParser.h"
#include <DebugUtil.h>
#include <ValueClamp.h>
#include <AglVector3.h>

string MeshNameScriptParser::separators = "_";
string MeshNameScriptParser::decimalSeparator = "d";
string MeshNameScriptParser::instantiate = "I";
string MeshNameScriptParser::connectionpoint = "CP";
string MeshNameScriptParser::spawnpoint = "SP";
string MeshNameScriptParser::light = "L";
string MeshNameScriptParser::pointlightType = "point";
string MeshNameScriptParser::spotlightType = "spot";
string MeshNameScriptParser::dirlightType = "dir";

pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> 
	MeshNameScriptParser::parse(const string& p_string)
{
	Token tokenVal = MESH;
	Data data;
	data.name = p_string;
	string instr = getInstruction(p_string);
	if (instr==instantiate)
	{
		tokenVal = INSTANTIATE;
		data.instanceSpecFilename = extractPart(p_string,0)+".agl";
		data.name = extractPart(p_string,1);
	}
	else if (instr==connectionpoint)
	{
		tokenVal = CONNECTIONPOINT;
		data.name = extractPart(p_string,0);
	}
	else if (instr==spawnpoint)
	{
		tokenVal = SPAWNPOINT;
		data.spawnSpecName = extractPart(p_string,0);
		data.name = extractPart(p_string,1);
	}
	else if (instr==light)
	{
		data.name = extractPart(p_string,0);
		if (data.name==pointlightType)
		{
			tokenVal = POINTLIGHT;
			data.lightSpec.type = LightCreationData::POINT;
		}
		else if (data.name==spotlightType)
		{
			tokenVal = SPOTLIGHT;
			data.lightSpec.type = LightCreationData::SPOT;
		}
		else
		{
			tokenVal = DIRLIGHT;
			data.lightSpec.type = LightCreationData::DIR;
		}
		// raw data
		string diffuse = extractPart(p_string,1);
		string specular = extractPart(p_string,2);
		string ambient = extractPart(p_string,3);
		string glossFloat = extractPart(p_string,4);
		string distFloat = extractPart(p_string,5);
		string pwrFloat = extractPart(p_string,6);
		string constAttFloat = extractPart(p_string,7);
		string linAttFloat = extractPart(p_string,8);
		string quadAttFloat = extractPart(p_string,9);
		// store converted in data
		getRGB(data.lightSpec.diffuse,diffuse);
		getRGB(data.lightSpec.specular,specular);
		getRGB(data.lightSpec.ambient,ambient);
		data.lightSpec.gloss = getFloatFromDecimalString(glossFloat);
		data.lightSpec.range = getFloatFromDecimalString(distFloat);
		data.lightSpec.power = getFloatFromDecimalString(pwrFloat);
		data.lightSpec.attenuation.x = getFloatFromDecimalString(constAttFloat);
		data.lightSpec.attenuation.y = getFloatFromDecimalString(linAttFloat);
		data.lightSpec.attenuation.z = getFloatFromDecimalString(quadAttFloat);
	}
	return pair<Data,Token>(data,tokenVal);
}

std::string MeshNameScriptParser::getInstruction( const string& p_string )
{
	return p_string.substr(0,p_string.find(separators));
}

std::string MeshNameScriptParser::extractPart( const string& p_string,int offset )
{
	int start=-1;
	for (int i=-1;i<offset;i++)
	{
		start=p_string.find(separators,start+1);
	}
	int s = clamp(start+1, 0, (int)p_string.size()-1);
	int suffixLength = max(0, (int)(p_string.size()-(p_string.find(separators,s))) );
	int l = (int)p_string.size()-suffixLength-s;
	string sub = p_string.substr(s,l);
	return sub;
}

float MeshNameScriptParser::getFloatFromDecimalString( const string& p_string )
{
	// replace decimal separator with a dot
	string withDot = p_string;
	withDot.replace(p_string.find(decimalSeparator),1,".");
	// convert to a float
	float convVal = (float)::atof(withDot.c_str());
	return convVal;
}

void MeshNameScriptParser::getRGB( AglVector3& p_outVec, const string& p_hex )
{
	// calculate components from hex in 0-255
	int hex = ::atoi(p_hex.c_str());
	int r = ( hex >> 16 ) & 0xFF;
	int g = ( hex >> 8 ) & 0xFF;
	int b = hex & 0xFF;
	// return a vector in 0-1 space
	p_outVec.x = r/255.0f;
	p_outVec.y = g/255.0f;
	p_outVec.z = b/255.0f;
}
