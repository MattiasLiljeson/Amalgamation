// =======================================================================================
//                                      Shader
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Shader
/// Detailed description.....
/// Created on: 27-11-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include <string>
#include <comdef.h>
#include <vector>
#include "D3DCompiler.h"
#include "Util.h"
#include "D3DException.h"
#include "ToString.h"
#include "Buffer.h"

using namespace std;

class Shader
{
public:
	Shader(const LPCWSTR& p_path, ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	virtual ~Shader();

	void apply();

	struct VertexShaderData
	{
		ID3D10Blob* compiledData;
		ID3D11VertexShader* data;
	};

	struct PixelShaderData
	{
		ID3D10Blob* compiledData;
		ID3D11PixelShader* data;
	};

	struct CBufferTest
	{
		float color[4];
	};

	Buffer<CBufferTest>* tempGetBufferPtr() {return m_CBufferPerFrame;}

protected:
	void initBuffers();
private:

	///-----------------------------------------------------------------------------------
	/// Creates a shader blob
	/// \param p_sourceFile
	/// \param p_entryPoint
	/// \param p_profile
	/// \param p_blob
	/// \returns void
	///-----------------------------------------------------------------------------------
	void createShader(const LPCWSTR &p_sourceFile, const string &p_entryPoint
		,const string &p_profile, ID3DBlob** p_blob);

	void createInputLayout();
private:
	// Input layout
	ID3D11InputLayout* m_inputLayout;

	// Sampler state
	ID3D11SamplerState* m_samplerState;

	// Shader programs
	VertexShaderData m_vertexShader;
	PixelShaderData m_pixelShader;

	// Per-program test buffers
	Buffer<CBufferTest>* m_CBufferPerFrame;
	// Buffer* m_vertexProgramCBuffer;

	// D3D
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};