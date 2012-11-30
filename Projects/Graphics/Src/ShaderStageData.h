// =======================================================================================
//                                      ShaderStageData
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Contains structs need for different stages of the shader program
///        
/// # ShaderStageData
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include "Util.h"

struct VSData
{
	ID3DBlob*			compiledData;
	ID3D11VertexShader* data;
	VSData()
	{
		compiledData = NULL;
		data = NULL;
	}
	~VSData()
	{
		SAFE_RELEASE(compiledData);
		SAFE_RELEASE(data);
	}
};

struct GSData
{
	ID3DBlob*				compiledData;
	ID3D11GeometryShader*	data;

	~GSData()
	{
		SAFE_RELEASE(compiledData);
		SAFE_RELEASE(data);
	}
};

struct DSData
{
	ID3DBlob*			compiledData;
	ID3D11DomainShader* data;

	~DSData()
	{
		SAFE_RELEASE(compiledData);
		SAFE_RELEASE(data);
	}
};
struct HSData
{
	ID3DBlob*			compiledData;
	ID3D11HullShader* data;

	~HSData()
	{
		SAFE_RELEASE(compiledData);
		SAFE_RELEASE(data);
	}
};
struct PSData
{
	ID3DBlob*			compiledData;
	ID3D11PixelShader*	data;

	PSData()
	{
		compiledData = NULL;
		data = NULL;
	}
	~PSData()
	{
		SAFE_RELEASE(compiledData);
		SAFE_RELEASE(data);
	}
};