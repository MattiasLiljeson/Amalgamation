// =======================================================================================
//                                      ShaderConfig
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains shader specific variables
///        
/// # ShaderConfig
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <comdef.h>
#include "ShaderStageData.h"

using namespace std;

struct ShaderConfig
{
public:
	ShaderConfig(const LPCWSTR& p_filePath, const string& p_entryPoint, 
		const string& p_profile)
	{
		filePath = p_filePath;
		entryPoint = p_entryPoint;
		profile = p_profile;
	}

	LPCWSTR getFilePath()
	{ 
		return filePath;
	}
	string getEntryPoint()
	{
		return entryPoint;
	}
	string getProfile()
	{
		return profile;
	}

private:
	LPCWSTR filePath;
	string entryPoint;
	string profile;
};