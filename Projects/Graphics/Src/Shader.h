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
#include "D3DCompiler.h"
#include "Util.h"
#include "D3DException.h"
#include <comdef.h>
#include "ToString.h"


class Shader
{
public:
	Shader(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext);
	virtual ~Shader();
	
	struct ShaderVertexProgramCBuffer
	{
		float test;
	};

	struct ShaderPixelProgramCBuffer
	{
		float test;
	};
protected:
	void initBuffers();

private:
	// Input layout
	ID3D11InputLayout* m_inputLayout;

	// Shader programs
	ID3D11VertexShader* m_vertexProgram;
	ID3D11PixelShader* m_pixelProgram;

	// Per-program test buffers
	ID3D11Buffer* m_pixelProgramCBuffer;
	ID3D11Buffer* m_vertexProgramCBuffer;

	// D3D
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};