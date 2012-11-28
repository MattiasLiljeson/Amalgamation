#include "Shader.h"



Shader::Shader(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	m_pixelProgramCBuffer = NULL;
	m_vertexProgramCBuffer = NULL;

	HRESULT res = S_OK;
	ID3DBlob*	blobShader = NULL;
	ID3DBlob*	blobError  = NULL;

	DWORD compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	// Compile the programs
	// vertex
	res = D3DCompileFromFile(L"Assets/Shaders/vertexTest.vs", 0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
								"VS", "vs_5_0", 
								compileFlags, 0, 
								&blobShader, &blobError);
	if (res != S_OK)
	{
		if (blobError!=NULL)
			throw D3DException(blobError,__FILE__,__FUNCTION__,__LINE__);
		else
			throw D3DException(res,__FILE__,__FUNCTION__,__LINE__);
	}

	// pixel
	res = D3DCompileFromFile(L"Assets/Shaders/pixelTest.ps", 0, 0,
								"PS", "ps_5_0", 
								compileFlags, 0, 
								&blobShader, &blobError);
	if (res != S_OK)
	{
		if (blobError!=NULL)
			throw D3DException(blobError,__FILE__,__FUNCTION__,__LINE__);
		else
			throw D3DException(res,__FILE__,__FUNCTION__,__LINE__);
	}


	// Initialize the buffers
	// initBuffers();

}

Shader::~Shader()
{
	SAFE_RELEASE(m_pixelProgram);
	SAFE_RELEASE(m_vertexProgram);
	SAFE_RELEASE(m_pixelProgramCBuffer);
	SAFE_RELEASE(m_vertexProgramCBuffer);
}

void Shader::initBuffers()
{
	
	// Set up vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth			= sizeof(ShaderVertexProgramCBuffer);
	bufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride	= 0;


	HRESULT res = m_device->CreateBuffer(&bufferDesc, NULL, &m_vertexProgramCBuffer);
	if (res != S_OK)
		throw D3DException("Could not initialize buffer for vertex program!",__FILE__,__FUNCTION__,__LINE__);

	
	// Set up pixel buffer
	bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth			= sizeof(ShaderPixelProgramCBuffer);
	bufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride	= 0;

	res = m_device->CreateBuffer(&bufferDesc, NULL, &m_pixelProgramCBuffer);
	if (res != S_OK)
		throw D3DException("Could not initialize buffer for pixel program!",__FILE__,__FUNCTION__,__LINE__);
}



