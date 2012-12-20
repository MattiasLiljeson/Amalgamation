#include "DeferredBaseShader.h"

/*
DeferredBaseShader::DeferredBaseShader(const LPCWSTR& p_path, ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;

	m_CBufferPerFrame = NULL;

	createShader(p_path, "VS", "vs_5_0",
		&m_vertexShader.compiledData);

	if ( FAILED (m_device->CreateVertexShader(
		m_vertexShader.compiledData->GetBufferPointer(),
		m_vertexShader.compiledData->GetBufferSize(), NULL,	&m_vertexShader.data)) )
	{
		throw D3DException("Couldn't create vertex shader",__FILE__,__FUNCTION__,__LINE__);
	}

	createShader(p_path, "PS", "ps_5_0",
		&m_pixelShader.compiledData);

	if ( FAILED (m_device->CreatePixelShader(
		m_pixelShader.compiledData->GetBufferPointer(),
		m_pixelShader.compiledData->GetBufferSize(), NULL, &m_pixelShader.data)) )
	{
		throw D3DException("Couldn't create pixel shader",__FILE__,__FUNCTION__,__LINE__);
	}

	m_samplerState = NULL;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc,sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	if ( FAILED (m_device->CreateSamplerState(&samplerDesc,&m_samplerState)) )
	{
		throw D3DException("Couldn't create sampler state",__FILE__,__FUNCTION__,__LINE__);
	}

	createInputLayout();
	initBuffers();
}
*/

DeferredBaseShader::DeferredBaseShader( ShaderInitStruct p_initData, 
									   Buffer<SimpleCBuffer>* p_cBuffer ) 
									   : ShaderBase(p_initData)
{
	m_perFrameBuffer = p_cBuffer;
}

DeferredBaseShader::~DeferredBaseShader()
{
	delete m_perFrameBuffer;
}

/*
void DeferredBaseShader::createShader( const LPCWSTR &p_sourceFile, const string &p_entryPoint ,
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

void DeferredBaseShader::createInputLayout()
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

void DeferredBaseShader::initBuffers()
{
	CBufferTest color={
		0.0f,1.0f,0.0f,1.0f,
	};

	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(CBufferTest);
	bufferDesc.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bufferDesc.NumElements = 1;
	bufferDesc.Type = BufferConfig::CONSTANT_BUFFER_PS;

	// Store description in config object
	BufferConfig* initConfig = new BufferConfig(bufferDesc);

	// Create buffer from config and data
	m_CBufferPerFrame = new Buffer<CBufferTest>(m_device,m_deviceContext,&color,initConfig);
}
*/
void DeferredBaseShader::apply()
{
	m_perFrameBuffer->apply();
	applyStages();
}

Buffer<SimpleCBuffer>* DeferredBaseShader::getPerFrameBufferPtr()
{
	return m_perFrameBuffer;
}
