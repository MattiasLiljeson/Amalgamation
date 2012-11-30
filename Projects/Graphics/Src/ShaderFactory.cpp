#include "ShaderFactory.h"

ShaderFactory::ShaderFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;
}

DeferredBaseShader* ShaderFactory::createDeferredBaseShader(const LPCWSTR& p_filePath)
{
	DeferredBaseShader* newDeferredBaseShader = NULL;
	ID3D11SamplerState* samplerState = NULL;
	ID3D11InputLayout* inputLayout = NULL;

	Buffer<SimpleCBuffer>* cBuffer;
	BufferConfig* initConfig  = NULL;

	VSData* vertexData = new VSData();
	PSData* pixelData = new PSData();

	///
	createAllShaderStages(p_filePath,vertexData,pixelData);
	///
	createSamplerState(&samplerState);
	///
	createVertexInputLayout(vertexData,&inputLayout);
	///

	ShaderInitStruct shaderInitData;

	shaderInitData.device = m_device;
	shaderInitData.deviceContext = m_deviceContext;
	shaderInitData.domainShader = NULL;
	shaderInitData.geometryShader = NULL;
	shaderInitData.hullShader = NULL;
	shaderInitData.vertexShader = vertexData;
	shaderInitData.pixelShader = pixelData;
	shaderInitData.samplerState = samplerState;
	shaderInitData.inputLayout = inputLayout;

	///
	SimpleCBuffer data={
		0.0f,1.0f,0.0f,1.0f
	};

	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(SimpleCBuffer);
	bufferDesc.Usage = BufferConfig::BUFFER_CPU_WRITE_DISCARD;
	bufferDesc.NumElements = 1;
	bufferDesc.Type = BufferConfig::CONSTANT_BUFFER_PS;

	initConfig = new BufferConfig(bufferDesc);

	cBuffer = new Buffer<SimpleCBuffer>(m_device,m_deviceContext,&data,initConfig);

	newDeferredBaseShader = new DeferredBaseShader(shaderInitData,cBuffer);

	return newDeferredBaseShader;
}

DeferredComposeShader* ShaderFactory::createDeferredComposeShader(const LPCWSTR& p_filePath)
{
	DeferredComposeShader* newDeferredComposeShader = NULL;
	ID3D11SamplerState* samplerState = NULL;
	ID3D11InputLayout* inputLayout = NULL;

	BufferConfig* initConfig  = NULL;

	VSData* vertexData = new VSData();
	PSData* pixelData = new PSData();

	///
	createAllShaderStages(p_filePath,vertexData,pixelData);
	///
	createSamplerState(&samplerState);
	///
	createVertexInputLayout(vertexData,&inputLayout);
	///

	ShaderInitStruct shaderInitData;

	shaderInitData.device = m_device;
	shaderInitData.deviceContext = m_deviceContext;
	shaderInitData.domainShader = NULL;
	shaderInitData.geometryShader = NULL;
	shaderInitData.hullShader = NULL;
	shaderInitData.vertexShader = vertexData;
	shaderInitData.pixelShader = pixelData;
	shaderInitData.samplerState = samplerState;
	shaderInitData.inputLayout = inputLayout;


	newDeferredComposeShader = new DeferredComposeShader(shaderInitData);

	return newDeferredComposeShader;
}

void ShaderFactory::compileShaderStage( const LPCWSTR &p_sourceFile, const string &p_entryPoint,
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

void ShaderFactory::createAllShaderStages( const LPCWSTR& p_filePath, VSData* p_vs/*=NULL*/,PSData* p_ps/*=NULL*/ )
{
	HRESULT hr = S_OK;
	compileShaderStage(p_filePath,"VS","vs_5_0",&p_vs->compiledData);

	hr = m_device->CreateVertexShader(p_vs->compiledData->GetBufferPointer(),
		p_vs->compiledData->GetBufferSize(), NULL, &p_vs->data);
	if(FAILED(hr))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);

	compileShaderStage(p_filePath,"PS","ps_5_0",&p_ps->compiledData);

	hr = m_device->CreatePixelShader(p_ps->compiledData->GetBufferPointer(),
		p_ps->compiledData->GetBufferSize(), NULL, &p_ps->data);
	if (FAILED(hr))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
}

void ShaderFactory::createSamplerState( ID3D11SamplerState** p_samplerState )
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc,sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerDesc,p_samplerState );
	if(FAILED(hr))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
}

void ShaderFactory::createVertexInputLayout( VSData* p_vs, ID3D11InputLayout** p_inputLayout )
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,	DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = m_device->CreateInputLayout(input,	(sizeof(input)/sizeof(input[0])), 
		p_vs->compiledData->GetBufferPointer(),
		p_vs->compiledData->GetBufferSize(), p_inputLayout);
	if ( FAILED(hr))
		throw D3DException(hr, __FILE__, __FUNCTION__, __LINE__);
}
