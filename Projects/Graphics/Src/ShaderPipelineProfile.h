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

#include "ShaderStageConfig.h"

struct ShaderPipelineProfile
{
	///-----------------------------------------------------------------------------------
	/// Separate shader stage configurations
	/// \param p_vertex
	/// \param p_pixel
	/// \param p_geometry
	/// \param p_hull
	/// \param p_domain
	/// \returns 
	///-----------------------------------------------------------------------------------
	ShaderPipelineProfile(ShaderStageConfig* p_vertex,ShaderStageConfig* p_pixel,
						  ShaderStageConfig* p_geometry=NULL,
						  ShaderStageConfig* p_hull=NULL,ShaderStageConfig* p_domain=NULL)
	{
		pixelShader		= p_pixel;
		vertexShader	= p_vertex;
		hullShader		= p_hull;
		geometryShader	= p_geometry;
		domainShader	= p_domain;
	}

	///-----------------------------------------------------------------------------------
	/// Shader stages with a shared path.
	/// \param p_sharedPath
	/// \param p_vertex
	/// \param p_pixel
	/// \param p_geometry
	/// \param p_hull
	/// \param p_domain
	/// \returns 
	///-----------------------------------------------------------------------------------
	ShaderPipelineProfile(const LPCWSTR& p_sharedPath,
						  ShaderStageConfig* p_vertex,ShaderStageConfig* p_pixel,
						  ShaderStageConfig* p_geometry=NULL,
						  ShaderStageConfig* p_hull=NULL,ShaderStageConfig* p_domain=NULL)
	{
		pixelShader		= p_pixel;
		vertexShader	= p_vertex;
		hullShader		= p_hull;
		geometryShader	= p_geometry;
		domainShader	= p_domain;
		setCommonShaderPath(p_sharedPath);
	}


	///-----------------------------------------------------------------------------------
	/// Shader stages with shared version.
	/// \param p_sharedVersion
	/// \param p_vertex
	/// \param p_pixel
	/// \param p_geometry
	/// \param p_hull
	/// \param p_domain
	/// \returns 
	///-----------------------------------------------------------------------------------
	ShaderPipelineProfile(const string& p_sharedVersion,
						  ShaderStageConfig* p_vertex,ShaderStageConfig* p_pixel,
						  ShaderStageConfig* p_geometry=NULL,
						  ShaderStageConfig* p_hull=NULL,ShaderStageConfig* p_domain=NULL)
	{
		pixelShader		= p_pixel;
		vertexShader	= p_vertex;
		hullShader		= p_hull;
		geometryShader	= p_geometry;
		domainShader	= p_domain;
		setCommonShaderVersion(p_sharedVersion);
	}

	///-----------------------------------------------------------------------------------
	/// Shader stages with shared version and path.
	/// \param p_sharedPath
	/// \param p_sharedVersion
	/// \param p_vertex
	/// \param p_pixel
	/// \param p_geometry
	/// \param p_hull
	/// \param p_domain
	/// \returns 
	///-----------------------------------------------------------------------------------
	ShaderPipelineProfile(const LPCWSTR& p_sharedPath,
						  const string& p_sharedVersion,
						  ShaderStageConfig* p_vertex,ShaderStageConfig* p_pixel,
						  ShaderStageConfig* p_geometry=NULL,
						  ShaderStageConfig* p_hull=NULL,ShaderStageConfig* p_domain=NULL)
	{
		pixelShader		= p_pixel;
		vertexShader	= p_vertex;
		hullShader		= p_hull;
		geometryShader	= p_geometry;
		domainShader	= p_domain;
		setCommonShaderPath(p_sharedPath);
		setCommonShaderVersion(p_sharedVersion);
	}


	~ShaderPipelineProfile()
	{
		delete	pixelShader;
		delete	vertexShader;	
		delete	hullShader;	
		delete	geometryShader;
		delete	domainShader;
	}


private:
	void setCommonShaderPath(const LPCWSTR& p_shaderPath)
	{
		pixelShader->filePath		= p_shaderPath;
		vertexShader->filePath		= p_shaderPath;
		geometryShader->filePath	= p_shaderPath;
		hullShader->filePath		= p_shaderPath;
		domainShader->filePath		= p_shaderPath;
	}
	void setCommonShaderVersion(const string& p_shaderVersion)
	{
		pixelShader->version		= p_shaderVersion;
		vertexShader->version		= p_shaderVersion;
		geometryShader->version		= p_shaderVersion;
		hullShader->version			= p_shaderVersion;
		domainShader->version		= p_shaderVersion;
	}
private:
	ShaderStageConfig* pixelShader;
	ShaderStageConfig* vertexShader;
	ShaderStageConfig* hullShader;
	ShaderStageConfig* geometryShader;
	ShaderStageConfig* domainShader;
};