#include "Deferred.h"

Deferred::Deferred(ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, 
				   int p_width, int p_height)
{
	m_device		= p_device;
	m_deviceContext = p_deviceContext;

	m_width		= p_width;
	m_height	= p_height;

	m_shaderFactory = new ShaderFactory(m_device,m_deviceContext);

	//m_baseShader	= new DeferredBaseShader(L"Assets/Shaders/deferredBase.hlsl", m_device, p_deviceContext);
	//m_composeShader	= new DeferredBaseShader(L"Assets/Shaders/deferredCompose.hlsl", m_device, p_deviceContext);
	m_vertexBuffer	= NULL;

	initDepthStencil();
	initGeomtryBuffers();
	initTestShaders();

	createFullScreenQuad();
}

Deferred::~Deferred()
{
	SAFE_RELEASE(m_depthStencilView);

	for (int i = 0; i < NUMBUFFERS; i++)
	{
		SAFE_RELEASE(m_gBuffers[i]);
		SAFE_RELEASE(m_gBuffersShaderResource[i]);
	}

	delete m_shaderFactory;
	delete m_baseShader;
	delete m_composeShader;
	delete m_vertexBuffer;
}

void Deferred::deferredBasePass()
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_deviceContext->OMSetRenderTargets(NUMBUFFERS,m_gBuffers,m_depthStencilView);
	
	m_vertexBuffer->apply();
	
	m_baseShader->getPerFrameBufferPtr()->accessBuffer.color[0] = 1.0f;
	m_baseShader->getPerFrameBufferPtr()->accessBuffer.color[1] = 0.0f;
	m_baseShader->getPerFrameBufferPtr()->update();

	m_baseShader->apply();

	m_deviceContext->Draw(6,0);
}

void Deferred::renderComposedImage()
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_deviceContext->PSSetShaderResources(0,1,&m_gBuffersShaderResource[DIFFUSE]);
	m_deviceContext->PSSetShaderResources(1,1,&m_gBuffersShaderResource[NORMAL]);
	//m_deviceContext->PSSetShaderResources(0,2,&m_gBuffersShaderResource[0]);

	m_vertexBuffer->apply();

	m_composeShader->apply();

	m_deviceContext->Draw(6,0);
}

void Deferred::clearBuffers()
{
	unMapGBuffers();
	float clearColor[] = {
		0.0f,0.5f,0.5f,1.0f
	};
	for (unsigned int i = 0; i < NUMBUFFERS; i++)
	{
		m_deviceContext->ClearRenderTargetView(m_gBuffers[i], clearColor);
	}

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Deferred::unMapGBuffers()
{
	ID3D11ShaderResourceView * nulz[NUMBUFFERS];
	for (int i=0; i<NUMBUFFERS; i++)
		nulz[i]=NULL;
	m_deviceContext->PSSetShaderResources(0,NUMBUFFERS,nulz);
	m_composeShader->apply();
}

void Deferred::initDepthStencil()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if ( FAILED( m_device->CreateTexture2D( &depthStencilDesc,NULL,&depthStencilTexture)))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if ( FAILED ( m_device->CreateDepthStencilView(depthStencilTexture,
		&depthStencilViewDesc,&m_depthStencilView)))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	ZeroMemory(&shaderResourceDesc,sizeof(shaderResourceDesc));
	shaderResourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceDesc.Texture2D.MipLevels = 1;

	if ( FAILED ( m_device->CreateShaderResourceView(depthStencilTexture,
		&shaderResourceDesc, &m_gBuffersShaderResource[DEPTH])))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);

	depthStencilTexture->Release();
}

void Deferred::initGeomtryBuffers()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* gBufferTextures[NUMBUFFERS];
	D3D11_TEXTURE2D_DESC gBufferDesc;
	ZeroMemory( &gBufferDesc, sizeof(gBufferDesc) );

	gBufferDesc.Width = m_width;
	gBufferDesc.Height = m_height;
	gBufferDesc.MipLevels = 1;
	gBufferDesc.ArraySize = 1;
	gBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	gBufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	gBufferDesc.SampleDesc.Count = 1;
	gBufferDesc.SampleDesc.Quality = 0;
	gBufferDesc.CPUAccessFlags = 0;
	gBufferDesc.MiscFlags = 0;

	for (unsigned int i = 0; i < NUMBUFFERS; i++){
		hr = m_device->CreateTexture2D(&gBufferDesc,NULL,&gBufferTextures[i]);		
		if (hr != S_OK)
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = gBufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (unsigned int i = 0; i < NUMBUFFERS; i++){
		hr = m_device->CreateRenderTargetView(gBufferTextures[i], &renderTargetViewDesc,
			&m_gBuffers[i]);
		if (hr != S_OK )
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	shaderResourceDesc.Format = gBufferDesc.Format;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MipLevels = 1;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;

	/************************************************************************/
	/* !!! Note that the for loop starts at index 1 since depthbuffer		*/
	/* already in init !!!													*/
	/************************************************************************/
	for (unsigned int i = DIFFUSE; i < NUMBUFFERS-1; i++){
		hr = m_device->CreateShaderResourceView(gBufferTextures[i],&shaderResourceDesc,
			&m_gBuffersShaderResource[i]);
		gBufferTextures[i]->Release();
		if (hr != S_OK )
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
	}
}

void Deferred::createFullScreenQuad( )
{
	PTVertex mesh[]= {
		{{ 1,	-1,	0},	{ 1, 1}},
		{{ -1,	-1,	0},	{ 0, 1}},
		{{ 1,	1,	0},	{ 1, 0}},

		{{ -1, -1,	0},	{ 0, 1}},
		{{ 1,	1,	0},	{ 1, 0}},
		{{ -1,	1,	0},	{ 0, 0}}
	};


	// Create description for buffer
	BufferConfig::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(PTVertex);
	bufferDesc.Usage = BufferConfig::BUFFER_DEFAULT;
	bufferDesc.NumElements = 6;
	bufferDesc.Type = BufferConfig::VERTEX_BUFFER;

	// Store description in config object
	BufferConfig* initConfig = new BufferConfig(bufferDesc);

	// Create buffer from config and data
	m_vertexBuffer = new Buffer<PTVertex>(m_device,m_deviceContext,&mesh[0],initConfig);
}

void Deferred::initTestShaders()
{
	m_baseShader = m_shaderFactory->createDeferredBaseShader(
		L"Assets/Shaders/deferredBase.hlsl");

	m_composeShader = m_shaderFactory->createDeferredComposeShader(
		L"Assets/Shaders/deferredCompose.hlsl");
}




