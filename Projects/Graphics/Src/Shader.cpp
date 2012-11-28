#include "Shader.h"

Shader::Shader(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	m_pixelProgramCBuffer = NULL;
	m_vertexProgramCBuffer = NULL;

	createShader(L"Assets/Shaders/vertexTest.vs", "VS", "vs_4_0",	
		&m_vertexShader.compiledData);

	if ( FAILED (m_device->CreateVertexShader(
		m_vertexShader.compiledData->GetBufferPointer(),
		m_vertexShader.compiledData->GetBufferSize(), NULL,	&m_vertexShader.data)) )
	{
		throw D3DException("Couldn't create vertex shader",__FILE__,__FUNCTION__,__LINE__);
	}

	createShader(L"Assets/Shaders/pixelTest.ps", "PS", "ps_4_0", 
		&m_pixelShader.compiledData);

	if ( FAILED (m_device->CreatePixelShader(
		m_pixelShader.compiledData->GetBufferPointer(),
		m_pixelShader.compiledData->GetBufferSize(), NULL, &m_pixelShader.data)) )
	{
		throw D3DException("Couldn't create pixel shader",__FILE__,__FUNCTION__,__LINE__);
	}

	createInputLayout();
	initBuffers();
}

Shader::~Shader()
{
	SAFE_RELEASE(m_pixelShader.data);
	SAFE_RELEASE(m_vertexShader.data);
	SAFE_RELEASE(m_pixelProgramCBuffer);
	SAFE_RELEASE(m_vertexProgramCBuffer);
}


void Shader::createShader( const LPCWSTR &p_sourceFile, const string &p_entryPoint ,
							 const string &p_profile, ID3DBlob** p_blob )
{
	HRESULT res = S_OK;
	
	ID3DBlob*	blobError  = NULL;
	ID3DBlob*	shaderBlob = NULL;

	*p_blob = NULL;
	
	DWORD compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags |= D3DCOMPILE_DEBUG;
#endif


	// Compile the programs
	// vertex
	res = D3DCompileFromFile(p_sourceFile, 0, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		(LPCTSTR)p_entryPoint.c_str(), (LPCTSTR)p_profile.c_str(), 
		compileFlags, 0, 
		&shaderBlob, &blobError);
	if ( FAILED(res) )
	{
		if (blobError!=NULL)
			throw D3DException(blobError,__FILE__,__FUNCTION__,__LINE__);
		else
			throw D3DException(res,__FILE__,__FUNCTION__,__LINE__);

		return;
	}

	*p_blob = shaderBlob;
}

void Shader::createInputLayout()
{
	HRESULT res = S_OK;

	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,	DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	if ( FAILED (m_device->CreateInputLayout(input, 
		(sizeof(input)/sizeof(input[0])), m_vertexShader.compiledData->GetBufferPointer(),
		m_vertexShader.compiledData->GetBufferSize(), &m_inputLayout) ) )
	{
		throw D3DException("Failed to create input layout.",__FILE__,__FUNCTION__,
			__LINE__);
	}

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


	if (FAILED (m_device->CreateBuffer(&bufferDesc, NULL, &m_vertexProgramCBuffer)) )
	{
		throw D3DException("Could not initialize buffer for vertex program!",__FILE__,
		__FUNCTION__,__LINE__);
		return;
	}


	// Set up pixel buffer
	bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth			= sizeof(ShaderPixelProgramCBuffer);
	bufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride	= 0;

	if ( FAILED(m_device->CreateBuffer(&bufferDesc, NULL, &m_pixelProgramCBuffer)) )
	{
		throw D3DException("Could not initialize buffer for pixel program!",__FILE__,
		__FUNCTION__,__LINE__);
		return;
	}
}

void Shader::apply()
{
	m_deviceContext->VSSetShader(m_vertexShader.data, 0, 0);
	m_deviceContext->PSSetShader(m_pixelShader.data, 0, 0);
	m_deviceContext->IASetInputLayout(m_inputLayout);
}
