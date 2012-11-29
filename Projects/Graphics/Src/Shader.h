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
	Shader(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
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
	
	struct ShaderVertexProgramCBuffer
	{
		float color[4];
	};

	struct ShaderPixelProgramCBuffer
	{
		float color[4];
	};
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

	// Shader programs
	VertexShaderData m_vertexShader;
	PixelShaderData m_pixelShader;

	// Per-program test buffers
	Buffer* m_pixelProgramCBuffer;
	Buffer* m_vertexProgramCBuffer;

	// D3D
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};