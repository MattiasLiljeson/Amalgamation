#include "MeshNameScriptParser.h"
#include <DebugUtil.h>
#include <ValueClamp.h>

string MeshNameScriptParser::separators = "_";
string MeshNameScriptParser::instantiate = "I";
string MeshNameScriptParser::connectionpoint = "CP";
string MeshNameScriptParser::spawnpoint = "SP";

pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> 
	MeshNameScriptParser::parse(const string& p_string)
{
	Token tokenVal = MESH;
	Data data = {p_string,""};
	string instr = getInstruction(p_string);
	if (instr==instantiate)
	{
		tokenVal = INSTANTIATE;
		data.filename = extractPart(p_string,0)+".agl";
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
		data.name = extractPart(p_string,0);
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
	int l = max(0, (int)p_string.size()-(p_string.find(separators,s)-1)-s );
	string sub = p_string.substr(s,l);
	return sub;
}
