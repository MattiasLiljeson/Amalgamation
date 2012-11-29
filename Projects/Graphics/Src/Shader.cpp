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
	delete m_pixelProgramCBuffer;
	delete m_vertexProgramCBuffer;
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
	float color[]={
		0.0f,1.0f,0.0f,1.0f,
	};

	Buffer::BUFFER_INIT_DESC vertexBufferDesc;
	vertexBufferDesc.ElementSize = sizeof(ShaderVertexProgramCBuffer);
	vertexBufferDesc.Usage = Buffer::BUFFER_CPU_WRITE;
	vertexBufferDesc.InitData = &color[0];
	vertexBufferDesc.NumElements = 1;
	vertexBufferDesc.Type = Buffer::CONSTANT_BUFFER_VS;

	m_vertexProgramCBuffer = new Buffer(m_device,m_deviceContext,vertexBufferDesc);

	Buffer::BUFFER_INIT_DESC pixelBufferDesc;
	pixelBufferDesc.ElementSize = sizeof(ShaderPixelProgramCBuffer);
	pixelBufferDesc.Usage = Buffer::BUFFER_CPU_WRITE;
	pixelBufferDesc.InitData = &color[0];
	pixelBufferDesc.NumElements = 1;
	pixelBufferDesc.Type = Buffer::CONSTANT_BUFFER_PS;

	m_pixelProgramCBuffer = new Buffer(m_device,m_deviceContext,pixelBufferDesc);
}

void Shader::apply()
{
	m_vertexProgramCBuffer->apply();
	m_pixelProgramCBuffer->apply();
	m_deviceContext->VSSetShader(m_vertexShader.data, 0, 0);
	m_deviceContext->PSSetShader(m_pixelShader.data, 0, 0);
	m_deviceContext->IASetInputLayout(m_inputLayout);
}
