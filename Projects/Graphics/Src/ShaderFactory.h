// =======================================================================================
//                                      ShaderFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the creation of all the different shaders
///        
/// # ShaderFactory
/// Detailed description.....
/// Created on: 30-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include <comdef.h>
#include <D3DCompiler.h>
#include "D3DException.h"
#include "ShaderInitStruct.h"
#include "ShaderStageData.h"
#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "Buffer.h"
#include "CBuffers.h"
#include "BufferConfig.h"

class ShaderFactory
{
public:
	ShaderFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	DeferredBaseShader* createDeferredBaseShader(const LPCWSTR& p_filePath);
	DeferredComposeShader* createDeferredComposeShader(const LPCWSTR& p_filePath);
private:
	void compileShaderStage(const LPCWSTR &p_sourceFile, const string &p_entryPoint, 
		const string &p_profile, ID3DBlob** p_blob);

	// common functionality
	void createAllShaderStages(const LPCWSTR& p_filePath,
		VSData* p_vs=NULL,PSData* p_ps=NULL);

	void createSamplerState(ID3D11SamplerState** p_samplerState);

	void createVertexInputLayout(VSData* p_vs, ID3D11InputLayout** p_inputLayout);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};