// =======================================================================================
//                                      ShaderStageConfig
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains information for a specific shader stage.
///        
/// # ShaderEntryProfile
/// Detailed description.....
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <string>
#include <wtypes.h>

using namespace std;

struct ShaderStageConfig
{
	ShaderStageConfig(const LPCWSTR& p_filePath, const string& p_entryPoint, 
		const string& p_version)
	{
		filePath = p_filePath;
		entryPoint = p_entryPoint;
		version = p_version;
	}

	ShaderStageConfig(const string& p_entryPoint, 
		const string& p_version)
	{
		filePath=L"";
		entryPoint = p_entryPoint;
		version = p_version;
	}

	ShaderStageConfig(const string& p_entryPoint)
	{
		filePath=L"";
		entryPoint = p_entryPoint;
		version="";
	}

	ShaderStageConfig(const LPCWSTR& p_filePath, const string& p_entryPoint)
	{
		filePath = p_filePath;
		entryPoint = p_entryPoint;
		version="";
	}

	LPCWSTR filePath;
	string entryPoint;
	string version;
};