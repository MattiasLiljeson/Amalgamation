#include "ShaderFactory.h"
#include "BufferFactory.h"
#include "D3DException.h"
#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "GUIShader.h"
#include "ParticleShader.h"
#include "ShadowShader.h"

ShaderFactory::ShaderFactory(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
							 D3D_FEATURE_LEVEL p_featureLevel)
{
	m_device = p_device;
	m_deviceContext = p_deviceContext;
	switch(m_device->GetFeatureLevel())
	{
	case D3D_FEATURE_LEVEL_10_1:
	case D3D_FEATURE_LEVEL_10_0:
		m_shaderModelVersion = "4_0"; break;
	default:
		m_shaderModelVersion = "5_0"; break;
	}
	m_bufferFactory = new BufferFactory(m_device,m_deviceContext);

	switch( p_featureLevel )
	{
	case D3D_FEATURE_LEVEL_10_0:
		m_shaderModelVersion = "4_0";
		break;

	case D3D_FEATURE_LEVEL_10_1:
		m_shaderModelVersion = "4_0";
		break;

	case D3D_FEATURE_LEVEL_11_0:
		m_shaderModelVersion = "5_0";
		break;

	case D3D_FEATURE_LEVEL_11_1:
		m_shaderModelVersion = "5_0";
		break;
	}
}


ShaderFactory::~ShaderFactory()
{
	delete m_bufferFactory;
}


DeferredBaseShader* ShaderFactory::createDeferredBaseShader(const LPCWSTR& p_filePath)
{
	DeferredBaseShader* newDeferredBaseShader = NULL;
	ID3D11SamplerState* samplerState = NULL;
	ID3D11InputLayout* inputLayout = NULL;

	VSData* vertexData = new VSData();
	PSData* pixelData = new PSData();

	vertexData->stageConfig = new ShaderStageConfig(p_filePath,"VS",m_shaderModelVersion);
	pixelData->stageConfig = new ShaderStageConfig(p_filePath,"PS",m_shaderModelVersion);

	createAllShaderStages(vertexData,pixelData);
	createSamplerState(&samplerState);
	createInstancedPNTTBVertexInputLayout(vertexData,&inputLayout);

	ShaderVariableContainer shaderInitData;
	createShaderInitData(&shaderInitData,inputLayout,vertexData,pixelData,samplerState);

	return new DeferredBaseShader(shaderInitData);
}

DeferredComposeShader* ShaderFactory::createDeferredComposeShader( const LPCWSTR& p_filePath )
{
	DeferredComposeShader*	newComposeShader = NULL;
	ID3D11SamplerState*		samplerState = NULL;
	ID3D11InputLayout*		inputLayout = NULL;
	ShaderVariableContainer shaderVariables;

	VSData* vertexData	= new VSData();
	PSData* pixelData	= new PSData();
	
	vertexData->stageConfig = new ShaderStageConfig(p_filePath,"VS",m_shaderModelVersion);
	pixelData->stageConfig = new ShaderStageConfig(p_filePath,"PS", m_shaderModelVersion);

	createAllShaderStages(vertexData, pixelData);
	createSamplerState(&samplerState);
	createPTVertexInputLayout(vertexData,&inputLayout);
	createShaderInitData(&shaderVariables,inputLayout,vertexData,pixelData,samplerState);

	return new DeferredComposeShader(shaderVariables);
}

DeferredBaseShader* ShaderFactory::createLightShader( const LPCWSTR& p_filePath )
{
	VSData* vertexData = new VSData();
	vertexData->stageConfig = new ShaderStageConfig(p_filePath, "VS", m_shaderModelVersion);

	PSData* pixelData = new PSData();
	pixelData->stageConfig = new ShaderStageConfig(p_filePath, "PS", m_shaderModelVersion);
	createAllShaderStages(vertexData,pixelData);
	
	ID3D11InputLayout* inputLayout = NULL;
	//createPTVertexInputLayout(vertexData,&inputLayout);
	createInstancedLightInputLayout( vertexData, &inputLayout );

	ID3D11SamplerState* samplerState = NULL;
	createSamplerState( &samplerState );

	ShaderVariableContainer shaderInitData;
	createShaderInitData( &shaderInitData, inputLayout, vertexData, pixelData, samplerState );

	//DeferredComposeShader* newDeferredComposeShader = NULL;
	//newDeferredComposeShader = new DeferredComposeShader(shaderInitData);
	//return newDeferredComposeShader;

	DeferredBaseShader* newDeferredBaseShader = NULL;
	newDeferredBaseShader = new DeferredBaseShader(shaderInitData);
	return newDeferredBaseShader;
}

GUIShader* ShaderFactory::createGUIShader( const LPCWSTR& p_filePath )
{
	GUIShader* guiShader = NULL;
	ID3D11SamplerState* samplerState = NULL;
	ID3D11InputLayout* inputLayout = NULL;

	VSData* vertexData = new VSData();
	PSData* pixelData = new PSData();

	vertexData->stageConfig = new ShaderStageConfig(p_filePath,"VS",m_shaderModelVersion);
	pixelData->stageConfig = new ShaderStageConfig(p_filePath,"PS",m_shaderModelVersion);

	createAllShaderStages(vertexData,pixelData);
	createSamplerState(&samplerState);
	createInstancedPNTTBVertexInputLayout(vertexData,&inputLayout);

	ShaderVariableContainer shaderInitData;
	createShaderInitData(&shaderInitData,inputLayout,vertexData,pixelData,samplerState);

	guiShader = new GUIShader(shaderInitData,
		m_bufferFactory->createSimpleCBuffer());
	return guiShader;
}

ParticleShader* ShaderFactory::createParticleShader( const LPCWSTR& p_filePath )
{
	ID3D11SamplerState* samplerState = NULL;
	ID3D11InputLayout* inputLayout = NULL;
	ShaderVariableContainer shaderInitData;

	VSData* vertexD		= new VSData();
	GSData* geometryD	= new GSData();
	PSData* pixelD		= new PSData();

	vertexD->stageConfig = new ShaderStageConfig(p_filePath,"VS",m_shaderModelVersion);
	geometryD->stageConfig = new ShaderStageConfig(p_filePath,"GS",m_shaderModelVersion);
	pixelD->stageConfig = new ShaderStageConfig(p_filePath,"PS",m_shaderModelVersion);

	createAllShaderStages(vertexD,pixelD,geometryD);
	createSamplerState(&samplerState);
	createParticleInputLayout(vertexD,&inputLayout);

	createShaderInitData(&shaderInitData, inputLayout, vertexD, pixelD, 
		samplerState, geometryD);

	return new ParticleShader(shaderInitData, m_bufferFactory->createParticleCBuffer());
}

ShadowShader* ShaderFactory::createShadowShader( const LPCWSTR& p_filePath ){
	ID3D11SamplerState* samplerState = NULL;
	ID3D11InputLayout* inputLayout = NULL;
	ShaderVariableContainer shaderInitData;

	VSData* vertexData = new VSData();

	vertexData->stageConfig = new ShaderStageConfig(p_filePath,"VS",m_shaderModelVersion);

	createAllShaderStages(vertexData);
	createInstancedPNTTBVertexInputLayout(vertexData, &inputLayout);
	createShaderInitData(&shaderInitData, inputLayout, vertexData);

	return new ShadowShader(shaderInitData, m_bufferFactory->createShadowBuffer());
}

void ShaderFactory::compileShaderStage( const LPCWSTR &p_sourceFile, 
									    const string &p_entryPoint, 
										const string &p_profile, ID3DBlob** p_blob )
{
	HRESULT res = S_OK;

	ID3DBlob*	blobError  = NULL;
	ID3DBlob*	shaderBlob = NULL;

	*p_blob = NULL;

	DWORD compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(DEBUG) || defined(_DEBUG)
	compileFlags |= D3DCOMPILE_DEBUG; 
	compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	//compileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else
	compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; 
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

void ShaderFactory::createAllShaderStages(VSData* p_vs/* =NULL */, 
										  PSData* p_ps/* =NULL */, 
										  GSData* p_gs/* =NULL */, 
										  HSData* p_hs/* =NULL */, 
										  DSData* p_ds/* =NULL */)
{
	bool pixelCompiled	= false;
	bool vertexCompiled = false;
	bool geometryCompiled = false;
	bool hullCompiled	= false;
	bool domainCompiled = false;

	if (p_vs)
	{
		HRESULT hr = S_OK;
		compileShaderStage(p_vs->stageConfig->filePath,p_vs->stageConfig->entryPoint,
			string("vs_")+p_vs->stageConfig->version,&p_vs->compiledData);

		hr = m_device->CreateVertexShader(p_vs->compiledData->GetBufferPointer(),
			p_vs->compiledData->GetBufferSize(), NULL, &p_vs->data);
		if(FAILED(hr))
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
		else
			vertexCompiled = true;
	}
	else
		throw D3DException("Missing vertex shader", __FILE__,__FUNCTION__,__LINE__);

	if(p_ps)
	{
		HRESULT hr = S_OK;
		compileShaderStage(p_ps->stageConfig->filePath,p_ps->stageConfig->entryPoint,
			string("ps_")+p_ps->stageConfig->version,&p_ps->compiledData);

		hr = m_device->CreatePixelShader(p_ps->compiledData->GetBufferPointer(),
			p_ps->compiledData->GetBufferSize(), NULL, &p_ps->data);
		if(FAILED(hr))
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
		else
			pixelCompiled = true;
	}

	if(vertexCompiled)
	{
		if (p_gs)
		{
			HRESULT hr = S_OK;
			compileShaderStage(p_gs->stageConfig->filePath,p_gs->stageConfig->entryPoint,
				string("gs_")+p_gs->stageConfig->version,&p_gs->compiledData);

			hr = m_device->CreateGeometryShader(p_gs->compiledData->GetBufferPointer(),
				p_gs->compiledData->GetBufferSize(), NULL, &p_gs->data);
			if(FAILED(hr))
				throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
			else
				geometryCompiled = true;
		}

		if (p_hs)
		{
			HRESULT hr = S_OK;
			compileShaderStage(p_hs->stageConfig->filePath,p_hs->stageConfig->entryPoint,
				string("hs_")+p_hs->stageConfig->version,&p_hs->compiledData);

			hr = m_device->CreateHullShader(p_hs->compiledData->GetBufferPointer(),
				p_hs->compiledData->GetBufferSize(), NULL, &p_hs->data);
			if(FAILED(hr))
				throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
			else
				hullCompiled = true;
		}
	
		if(p_ds && p_hs)
		{
			HRESULT hr = S_OK;
			compileShaderStage(p_ds->stageConfig->filePath,p_ds->stageConfig->entryPoint,
				string("ds_")+p_ds->stageConfig->version,&p_ds->compiledData);

			hr = m_device->CreateDomainShader(p_ds->compiledData->GetBufferPointer(),
				p_ds->compiledData->GetBufferSize(), NULL, &p_ds->data);
			if(FAILED(hr))
				throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
			else 
				domainCompiled = true;
		}
		else if(hullCompiled)
			throw D3DException("Invalid shader stage config",__FILE__,__FUNCTION__,
			__LINE__);
	}
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


void ShaderFactory::createShaderInitData(ShaderVariableContainer* p_shaderInitData, 
										 ID3D11InputLayout* p_inputLayout,
										 VSData* p_vsd, PSData* p_psd/* =NULL */,
										 ID3D11SamplerState* p_samplerState/* =NULL */,
										 GSData* p_gsd/* =NULL */, 
										 HSData* p_hsd/* =NULL */, 
										 DSData* p_dsd/* =NULL */)
{
	p_shaderInitData->deviceContext = m_deviceContext;
	p_shaderInitData->inputLayout	= p_inputLayout;
	p_shaderInitData->vertexShader	= p_vsd;
	p_shaderInitData->hullShader	= p_hsd;
	p_shaderInitData->domainShader	= p_dsd;
	p_shaderInitData->geometryShader = p_gsd;
	p_shaderInitData->pixelShader	= p_psd;
	p_shaderInitData->samplerState = p_samplerState;
}


void ShaderFactory::createPTVertexInputLayout(VSData* p_vs, 
											  ID3D11InputLayout** p_inputLayout )
{
	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,	DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	constructInputLayout(input,sizeof(input)/sizeof(input[0]),p_vs,p_inputLayout);
}

void ShaderFactory::createPNTVertexInputLayout(VSData* p_vs, 
											   ID3D11InputLayout** p_inputLayout )
{
	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},		
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,	DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	constructInputLayout(input,sizeof(input)/sizeof(input[0]),p_vs,p_inputLayout);
}

void ShaderFactory::createInstancedPNTVertexInputLayout( VSData* p_vs, 
														ID3D11InputLayout** p_inputLayout)
{
	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},		
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,	DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
		D3D11_INPUT_PER_VERTEX_DATA, 0},


		{"INSTANCETRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};
	constructInputLayout(input,sizeof(input)/sizeof(input[0]),p_vs,p_inputLayout);
}


void ShaderFactory::createPNTTBVertexInputLayout( VSData* p_vs, 
												 ID3D11InputLayout** p_inputLayout )
{
	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},		
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,	DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	constructInputLayout(input,sizeof(input)/sizeof(input[0]),p_vs,p_inputLayout);
}

void ShaderFactory::createInstancedPNTTBVertexInputLayout( VSData* p_vs, 
														  ID3D11InputLayout** p_inputLayout )
{
	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},		
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0,	DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, 
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},


		{"INSTANCETRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};
	constructInputLayout(input,sizeof(input)/sizeof(input[0]),p_vs,p_inputLayout);
}

void ShaderFactory::createInstancedLightInputLayout( VSData* p_vertexShader,
													ID3D11InputLayout** p_inputLayout)
{
	D3D11_INPUT_ELEMENT_DESC input[] = {
		// Per vertex
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},

		{"INSTANCETRANSFORM", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTANCETRANSFORM", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 
		D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},

		// Per instance
		{"RANGE", 0, DXGI_FORMAT_R32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"LIGHTDIR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"ATTENUATION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"SPOTPOWER", 0, DXGI_FORMAT_R32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},

		{"AMBIENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"DIFFUSE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"SPECULAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},

		{"ENABLED", 0, DXGI_FORMAT_R32_SINT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"TYPE", 0, DXGI_FORMAT_R32_SINT, 1, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_INSTANCE_DATA, 1},

	};

	int elementCnt = sizeof(input)/sizeof(input[0]) ; //Will this work for both RGB and RGBA? Mattias L
	constructInputLayout(input, elementCnt, p_vertexShader, p_inputLayout);
}

void ShaderFactory::createParticleInputLayout( VSData* p_vs, 
											  ID3D11InputLayout** p_inpuyLayout )
{
	D3D11_INPUT_ELEMENT_DESC input[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"ANGULARVELOCITY", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	constructInputLayout(input,sizeof(input)/sizeof(input[0]),p_vs,p_inpuyLayout);
}

void ShaderFactory::constructInputLayout(const D3D11_INPUT_ELEMENT_DESC* p_inputDesc, 
										 UINT p_numberOfElements,
										 VSData* p_vs, ID3D11InputLayout** p_inputLayout )
{
	HRESULT hr = m_device->CreateInputLayout(
		p_inputDesc, 
		p_numberOfElements, 
		p_vs->compiledData->GetBufferPointer(),
		p_vs->compiledData->GetBufferSize(),
		p_inputLayout);

	if ( FAILED(hr) )
		throw D3DException(hr, __FILE__, __FUNCTION__, __LINE__);
}





