#include "MeshNameScriptParser.h"
#include <DebugUtil.h>
#include <ValueClamp.h>
#include <AglVector3.h>

string MeshNameScriptParser::separators = "_";
string MeshNameScriptParser::decimalSeparator = "d";
string MeshNameScriptParser::instantiate = "I";
string MeshNameScriptParser::connectionpoint = "CP";
string MeshNameScriptParser::spawnpoint = "SP";
/*string MeshNameScriptParser::light = "L";*/
string MeshNameScriptParser::lightpoint = "LP";
string MeshNameScriptParser::lightdir = "LD";
string MeshNameScriptParser::lightspot = "LS";
// string MeshNameScriptParser::lighthex = "LX";
// string MeshNameScriptParser::pointlightType = "point";
// string MeshNameScriptParser::spotlightType = "spot";
// string MeshNameScriptParser::dirlightType = "dir";

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
	else if (instr==lightpoint || instr==lightdir || instr==lightspot)
	{
		data.name = extractPart(p_string,0);
		if ( instr==lightpoint )
		{
			tokenVal = POINTLIGHT;
			data.lightSpec.type = LightCreationData::POINT;
		}
		else if ( instr==lightspot )
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
		int offset=0;
		// if ( instr==light ) // rgb
		// {
			float diffuseR = getFloatFromDecimalString(extractPart(p_string,0));
			float diffuseG = getFloatFromDecimalString(extractPart(p_string,1));
			float diffuseB = getFloatFromDecimalString(extractPart(p_string,2));
// 			float specR = getFloatFromDecimalString(extractPart(p_string,4));
// 			float specG = getFloatFromDecimalString(extractPart(p_string,5));
// 			float specB = getFloatFromDecimalString(extractPart(p_string,6));
// 			float ambientR = getFloatFromDecimalString(extractPart(p_string,7));
// 			float ambientG = getFloatFromDecimalString(extractPart(p_string,8));
// 			float ambientB = getFloatFromDecimalString(extractPart(p_string,9));
			// store data
			data.lightSpec.diffuse = AglVector3(diffuseR,diffuseG,diffuseB);

			// hardcode:
			data.lightSpec.specular = AglVector3(1.0f,1.0f,1.0f);
			data.lightSpec.ambient = AglVector3(0.0f,0.0f,0.0f);
// 			data.lightSpec.specular = AglVector3(specR,specG,specB);
// 			data.lightSpec.ambient = AglVector3(ambientR,ambientG,ambientB);
			// for further data when rgb
			// offset=6;
		// }
// 		else // hex
// 		{
// 			string diffuse = extractPart(p_string,1);
// 			string specular = extractPart(p_string,2);
// 			string ambient = extractPart(p_string,3);
// 			// store hex as rgb
// 			getRGB(data.lightSpec.diffuse,diffuse);
// 			getRGB(data.lightSpec.specular,specular);
// 			getRGB(data.lightSpec.ambient,ambient);
// 			// no offset when hex
// 		}
		// string glossFloat = extractPart(p_string,offset+3);
		string distFloat = extractPart(p_string,offset+3);
		string pwrFloat = extractPart(p_string,offset+4);
		// string constAttFloat = extractPart(p_string,offset+6);
		string linAttFloat = extractPart(p_string,offset+5);
		string quadAttFloat = extractPart(p_string,offset+6);
		// store converted in data
		data.lightSpec.gloss = 1.0f;
		data.lightSpec.range = getFloatFromDecimalString(distFloat);
		data.lightSpec.power = getFloatFromDecimalString(pwrFloat);
		data.lightSpec.attenuation.x = 0.0f;
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
	size_t pos = p_string.find(decimalSeparator);
	if (pos!=string::npos)
		withDot.replace(p_string.find(decimalSeparator),1,".");
	// convert to a float
	float convVal = 0.0f;
	if (withDot!=".") // keep as zero if string is only dot
		convVal=(float)::atof(withDot.c_str());
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
