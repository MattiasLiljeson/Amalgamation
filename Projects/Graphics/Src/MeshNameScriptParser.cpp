#include "MeshNameScriptParser.h"

string MeshNameScriptParser::separator = "_"; // TODO: Change to '.' later!
string MeshNameScriptParser::instantiate = "I";
string MeshNameScriptParser::connectionpoint = "CP";
string MeshNameScriptParser::spawnpoint = "SP";

pair<MeshNameScriptParser::Data,MeshNameScriptParser::Token> 
	MeshNameScriptParser::parse(const string& p_string)
{
	Token tokenVal = MESH;
	Data data = {"",""};
	string instr = getInstruction(p_string);
	if (instr==instantiate)
	{
		tokenVal = INSTANTIATE;
		data.filename = getFileName(p_string);
		data.name = getUniqueName(p_string,1);
	}
	else if (instr==connectionpoint)
	{
		tokenVal = CONNECTIONPOINT;
		data.name = getUniqueName(p_string,0);
	}
	else if (instr==spawnpoint)
	{
		tokenVal = SPAWNPOINT;
		data.name = getUniqueName(p_string,0);
	}
	return pair<Data,Token>(data,tokenVal);
}

std::string MeshNameScriptParser::getInstruction( const string& p_string )
{
	return p_string.substr(0,p_string.find(separator));
}

std::string MeshNameScriptParser::getFileName( const string& p_string )
{
	int start=p_string.find(separator);
	return p_string.substr(start,max(0,(int)p_string.find(separator,start+1)));
}

std::string MeshNameScriptParser::getUniqueName( const string& p_string,int offset )
{
	int start=-1;
	for (int i=-1;i<offset;i++)
	{
		start=p_string.find(separator,start+1);
	}
	
	return p_string.substr(min(start+1,(int)p_string.size()-1),
							max(0,(int)p_string.find(separator,start+1)));
}
