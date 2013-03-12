#include "AntTweakBarWrapper.h"
#include "BufferFactory.h"
#include "DeferredRenderer.h"
#include "ShaderFactory.h"
#include "Texture.h"

#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "DeferredAnimatedBaseShader.h"
#include "DeferredTessAnimatedBaseShader.h"
#include "DeferredTessBaseShader.h"
#include "LightShader.h"
#include "GUIShader.h"


DeferredRenderer::DeferredRenderer(
	ID3D11Device* p_device, ID3D11DeviceContext* p_deviceContext, int p_width, 
	int p_height, bool p_enableHdr, bool p_enableEffects )
{
	m_device		= p_device;
	m_deviceContext = p_deviceContext;

	m_width			= p_width;
	m_height		= p_height;
	m_enableHdr		= p_enableHdr;
	m_enableEffects = p_enableEffects;

	m_shaderFactory = new ShaderFactory(m_device,m_deviceContext, 
		m_device->GetFeatureLevel());

	
	m_depthStencilView = NULL;
	m_srvDepth = NULL;
	for( int i=0; i<RenderTargets_CNT; i++ )
	{
		m_rtvGBuffers[i] = NULL;
		m_srvGBuffers[i] = NULL;
		m_rtvDofBuffers[i] = NULL;
		m_srvDofBuffers[i] = NULL;
	}

	m_bufferFactory = new BufferFactory(m_device,m_deviceContext);

	initRendertargetsAndDepthStencil( m_width, m_height );

	initShaders();
	initSSAO();
	initFullScreenQuad();

	RenderStateHelper::fillBlendStateList( m_device, m_blendStates );
	m_currentBlendStateType = BlendState::DEFAULT;
	m_blendMask = 0xffffffff;
	for (int i=0;i<4;i++) m_blendFactors[i]=1;

	RenderStateHelper::fillRasterizerStateList( m_device, m_rasterizerStates );
	m_currentRasterizerStateType = RasterizerState::DEFAULT;

	RenderStateHelper::fillSamplerStateList( m_device, m_samplerStates );
	setSamplerStates();
}

DeferredRenderer::~DeferredRenderer()
{
	releaseRenderTargetsAndDepthStencil();

	for (unsigned int i = 0; i < m_blendStates.size(); i++){
		SAFE_RELEASE(m_blendStates[i]);
	}

	for (unsigned int i = 0; i < m_rasterizerStates.size(); i++){
		SAFE_RELEASE(m_rasterizerStates[i]);
	}

	delete m_shaderFactory;
	delete m_bufferFactory;
	delete m_baseShader;
	delete m_lightShader;
	delete m_effectShader;
	delete m_lowResGenerationShader;
	delete m_composeShader;
	delete m_fullscreenQuad;
	delete m_animatedBaseShader;
	delete m_tessAnimatedBaseShader;
	delete m_tessBaseShader;
}

void DeferredRenderer::initRendertargetsAndDepthStencil( int p_width, int p_height )
{
	m_width = p_width;
	m_height = p_height;

	initDepthStencil();
	initGeometryBuffers();
	initLightBuffers();
	initDofBuffers();
}

void DeferredRenderer::releaseRenderTargetsAndDepthStencil()
{
	// Release all buffers
	SAFE_RELEASE(m_depthStencilView);

	for (int i = 0; i < RenderTargets_CNT; i++)
	{
		SAFE_RELEASE(m_rtvGBuffers[i]);
		SAFE_RELEASE(m_srvGBuffers[i]);

		SAFE_RELEASE(m_rtvDofBuffers[i]);
		SAFE_RELEASE(m_srvDofBuffers[i]);
	}
}

void DeferredRenderer::setBasePassRenderTargets()
{
	m_deviceContext->OMSetRenderTargets(BASESHADERS,m_rtvGBuffers,m_depthStencilView);
}

void DeferredRenderer::setLightRenderTargets()
{
	m_deviceContext->OMSetRenderTargets( 2, &m_rtvGBuffers[RenderTargets_LIGHT_DIFFUSE], NULL );
}

void DeferredRenderer::setDofRenderTargets()
{
	m_deviceContext->OMSetRenderTargets( RenderTargets_CNT, m_rtvDofBuffers, NULL );

}

void DeferredRenderer::generateEffects()
{
	m_effectShader->setSSAOBufferData(m_ssaoData);
	m_effectShader->apply();
	m_fullscreenQuad->apply();

	m_deviceContext->Draw(6,0);
}

void DeferredRenderer::generateLowRes()
{
	m_lowResGenerationShader->setSSAOBufferData(m_ssaoData);
	m_lowResGenerationShader->apply();
	m_fullscreenQuad->apply();

	m_deviceContext->Draw(6,0);
}

void DeferredRenderer::renderComposeStage()
{
	m_composeShader->setSSAOBufferData(m_ssaoData);
	m_composeShader->apply();
	m_fullscreenQuad->apply();

	m_deviceContext->Draw(6,0);
}

void DeferredRenderer::setViewPortSize( float p_width, float p_height )
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width	= p_width;
	vp.Height	= p_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1,&vp);
}


void DeferredRenderer::mapNormal(ID3D11ShaderResourceView* p_shadowMap)
{	
	m_deviceContext->PSSetShaderResources( 1, 1, &m_srvGBuffers[RenderTargets_NORMAL] );

	//m_deviceContext->PSSetShaderResources( 4, 1, &p_shadowMap);
}

void DeferredRenderer::mapNormal()
{	
	m_deviceContext->PSSetShaderResources( 1, 1, &m_srvGBuffers[RenderTargets_NORMAL] );
}

void DeferredRenderer::unmapNormal()
{
	ID3D11ShaderResourceView* nulz = NULL;
	m_deviceContext->PSSetShaderResources( 1, 1, &nulz);
}

void DeferredRenderer::mapGbuffers(){
	m_deviceContext->PSSetShaderResources( 0, 1, &m_srvGBuffers[RenderTargets_DIFFUSE] );
	m_deviceContext->PSSetShaderResources( 1, 1, &m_srvGBuffers[RenderTargets_NORMAL] );
	m_deviceContext->PSSetShaderResources( 2, 1, &m_srvGBuffers[RenderTargets_SPECULAR] );
	m_deviceContext->PSSetShaderResources( 3, 1, &m_srvGBuffers[RenderTargets_LIGHT_DIFFUSE] );
	m_deviceContext->PSSetShaderResources( 4, 1, &m_srvGBuffers[RenderTargets_LIGHT_SPEC] );
}

void DeferredRenderer::unmapGbuffers(){
	ID3D11ShaderResourceView* nulz = NULL;
	m_deviceContext->PSSetShaderResources( 0, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 1, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 2, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 3, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 4, 1, &nulz );
}

void DeferredRenderer::mapDofBuffers(){
	m_deviceContext->PSSetShaderResources( 5, 1, &m_srvDofBuffers[RenderTargets_DIFFUSE] );
	m_deviceContext->PSSetShaderResources( 6, 1, &m_srvDofBuffers[RenderTargets_NORMAL] );
	m_deviceContext->PSSetShaderResources( 7, 1, &m_srvDofBuffers[RenderTargets_SPECULAR] );
	m_deviceContext->PSSetShaderResources( 8, 1, &m_srvDofBuffers[RenderTargets_LIGHT_DIFFUSE] );
	m_deviceContext->PSSetShaderResources( 9, 1, &m_srvDofBuffers[RenderTargets_LIGHT_SPEC] );
}

void DeferredRenderer::unmapDofBuffers(){
	ID3D11ShaderResourceView* nulz = NULL;

	m_deviceContext->PSSetShaderResources( 5, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 6, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 7, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 8, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 9, 1, &nulz );
}


void DeferredRenderer::mapDepth()
{
	m_deviceContext->PSSetShaderResources( DEPTH_IDX, 1, &m_srvDepth );
}

void DeferredRenderer::unmapDepth()
{
	ID3D11ShaderResourceView* nulz = NULL;
	m_deviceContext->PSSetShaderResources( DEPTH_IDX, 1, &nulz );
}

void DeferredRenderer::clearRenderTargets()
{
	unmapAllBuffers();
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for( unsigned int i=0; i<RenderTargets_CNT; i++ ) {
		m_deviceContext->ClearRenderTargetView( m_rtvGBuffers[i], clearColor );
		m_deviceContext->ClearRenderTargetView( m_rtvDofBuffers[i], clearColor );
	}

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11DepthStencilView* DeferredRenderer::getDepthStencil()
{
	return m_depthStencilView;
}

ID3D11ShaderResourceView*const* DeferredRenderer::getShaderResourceView( RenderTargets p_target )
{
	return &m_srvGBuffers[p_target];
}

void DeferredRenderer::setBlendState(BlendState::Mode p_state)
{
	unsigned int idx = static_cast<unsigned int>(p_state);
	m_deviceContext->OMSetBlendState( m_blendStates[idx], m_blendFactors, m_blendMask );
	m_currentBlendStateType = p_state;
}

void DeferredRenderer::setBlendFactors( float p_red, float p_green, float p_blue, 
									   float p_alpha )
{
	m_blendFactors[0]=p_red;
	m_blendFactors[1]=p_green;
	m_blendFactors[2]=p_blue;
	m_blendFactors[3]=p_alpha;
}

void DeferredRenderer::setBlendFactors( float p_oneValue )
{
	for (int i=0;i<4;i++)
		m_blendFactors[i]=p_oneValue;
}

void DeferredRenderer::setBlendMask( UINT p_mask )
{
	m_blendMask = p_mask;
}

BlendState::Mode DeferredRenderer::getCurrentBlendStateType() 
{
	return m_currentBlendStateType;
}

void DeferredRenderer::setRasterizerStateSettings( RasterizerState::Mode p_state )
{
	m_deviceContext->RSSetState( m_rasterizerStates[p_state] );
	m_currentRasterizerStateType = p_state;
}

RasterizerState::Mode DeferredRenderer::getCurrentRasterizerStateType()
{
	return m_currentRasterizerStateType;
}

void DeferredRenderer::setSamplerStates()
{
	m_deviceContext->PSSetSamplers( 0, SamplerState::SamplerState_CNT, m_samplerStates );
}

DeferredBaseShader* DeferredRenderer::getDeferredBaseShader()
{
	return m_baseShader;
}

DeferredAnimatedBaseShader* DeferredRenderer::getDeferredAnimatedBaseShader()
{
	return m_animatedBaseShader;
}

DeferredTessBaseShader* DeferredRenderer::getDeferredTessBaseShader()
{
	return m_tessBaseShader;
}

DeferredTessAnimatedBaseShader* DeferredRenderer::getDeferredTessAnimatedBaseShader()
{
	return m_tessAnimatedBaseShader;
}

LightShader* DeferredRenderer::getDeferredLightShader()
{
	return m_lightShader;
}

void DeferredRenderer::hookUpAntTweakBar()
{
	AntTweakBarWrapper::getInstance()->addWriteVariable(
		AntTweakBarWrapper::GRAPHICS,"Scale",TwType::TW_TYPE_FLOAT,&m_ssaoData.scale,
		"group=SSAO min=0 max=10 step=0.001");
	AntTweakBarWrapper::getInstance()->addWriteVariable(
		AntTweakBarWrapper::GRAPHICS,"Bias",TwType::TW_TYPE_FLOAT,&m_ssaoData.bias,
		"group=SSAO step=0.001");
	AntTweakBarWrapper::getInstance()->addWriteVariable(
		AntTweakBarWrapper::GRAPHICS,"Intensity",TwType::TW_TYPE_FLOAT,&m_ssaoData.intensity,
		"group=SSAO step=0.001");
	AntTweakBarWrapper::getInstance()->addWriteVariable(
		AntTweakBarWrapper::GRAPHICS,"SampleRad",TwType::TW_TYPE_FLOAT,&m_ssaoData.sampleRadius,
		"group=SSAO step=0.001");
	AntTweakBarWrapper::getInstance()->addWriteVariable(
		AntTweakBarWrapper::GRAPHICS,"Epsilon",TwType::TW_TYPE_FLOAT,&m_ssaoData.epsilon,
		"group=SSAO step=0.001");

	AntTweakBarWrapper::getInstance()->addWriteVariable(
		AntTweakBarWrapper::GRAPHICS,"cocFac",TwType::TW_TYPE_FLOAT,&m_ssaoData.cocFactor,
		"group=SSAO step=0.01");
}

void DeferredRenderer::initDepthStencil()
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

	HRESULT createTexHr = m_device->CreateTexture2D( &depthStencilDesc,NULL,&depthStencilTexture);
	checkHr( createTexHr, __FILE__, __FUNCTION__, __LINE__ );


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HRESULT createDepthStencilViewHr = m_device->CreateDepthStencilView(
		depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView );
	checkHr( createDepthStencilViewHr, __FILE__, __FUNCTION__, __LINE__ );


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	ZeroMemory(&shaderResourceDesc,sizeof(shaderResourceDesc));
	shaderResourceDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceDesc.Texture2D.MipLevels = 1;

	HRESULT createDepthShaderResourceView = m_device->CreateShaderResourceView(
		depthStencilTexture, &shaderResourceDesc, &m_srvDepth );
	checkHr( createDepthShaderResourceView, __FILE__, __FUNCTION__, __LINE__ );


	depthStencilTexture->Release();
}

void DeferredRenderer::initGeometryBuffers()
{
	// From first gbuffer to last
	int firstIdx = RenderTargets_DIFFUSE;
	int lastIdx = RenderTargets_SPECULAR;

	for( unsigned int i=firstIdx; i<=lastIdx; i++ ) {
		createSrvAndRtv( &m_srvGBuffers[i], &m_rtvGBuffers[i], m_width, m_height,
			DXGI_FORMAT_R8G8B8A8_UNORM );
		createSrvAndRtv( &m_srvDofBuffers[i], &m_rtvDofBuffers[i], m_width/4, m_height/4,
			DXGI_FORMAT_R8G8B8A8_UNORM );
	}
}

void DeferredRenderer::initLightBuffers()
{
	// From first light to last
	int firstIdx = RenderTargets_LIGHT_DIFFUSE;
	int lastIdx = RenderTargets_LIGHT_SPEC;

	for( unsigned int i=firstIdx; i<=lastIdx; i++ ) {
		createSrvAndRtv( &m_srvGBuffers[i], &m_rtvGBuffers[i], m_width, m_height,
			DXGI_FORMAT_R16G16B16A16_UNORM );
		createSrvAndRtv( &m_srvDofBuffers[i], &m_rtvDofBuffers[i], m_width/4, m_height/4,
			DXGI_FORMAT_R16G16B16A16_UNORM );
	}
}


void DeferredRenderer::initDofBuffers()
{
	// From first gbuffer to last
	int firstIdx = RenderTargets_DIFFUSE;
	int lastIdx = RenderTargets_LIGHT_SPEC;

	for( unsigned int i=firstIdx; i<=lastIdx; i++ ) {
		//createSrvAndRtv( &m_dofBuffersShaderResource[i], &m_dofBuffers[i], m_width/16, m_height/16,
		//	DXGI_FORMAT_R8G8B8A8_UNORM );
	}
	//m_srvDofBuffers[RenderTargets_DEPTH] = m_srvDofBuffers[RenderTargets_DEPTH];
	//m_srvDofBuffers[RenderTargets_DEPTH] = NULL;

	//m_dofBuffers[RenderTargets_DEPTH] = m_gBuffers[RenderTargets_DEPTH];
	//m_dofBuffers[RenderTargets_DEPTH] = NULL;
}

void DeferredRenderer::initShaders()
{
	m_baseShader = m_shaderFactory->createDeferredBaseShader(
		L"Shaders/Game/deferredBaseVS.hlsl", L"Shaders/Game/deferredBasePS.hlsl");

	m_tessBaseShader = m_shaderFactory->createDeferredTesselationBaseShader(
		L"Shaders/Game/deferredBaseTessleationVS.hlsl", L"Shaders/Game/deferredBaseTessleationHS.hlsl",
		L"Shaders/Game/deferredBaseTessleationDS.hlsl", L"Shaders/Game/deferredBasePS.hlsl");

	m_effectShader = m_shaderFactory->createDeferredComposeShader(
		L"Shaders/Game/effect.hlsl");

	m_lowResGenerationShader = m_shaderFactory->createDeferredComposeShader(
		L"Shaders/Game/lowResGeneration.hlsl");

	m_animatedBaseShader = m_shaderFactory->createDeferredAnimatedShader(
		L"Shaders/Game/deferredBaseAnimatedVS.hlsl", L"Shaders/Game/deferredBasePS.hlsl");

	m_tessAnimatedBaseShader = m_shaderFactory->createDeferredTessAnimatedShader(
		L"Shaders/Game/deferredAnimatedTesselationVS.hlsl",
		L"Shaders/Game/deferredBaseTessleationHS.hlsl",
		L"Shaders/Game/deferredBaseTessleationDS.hlsl", L"Shaders/Game/deferredBasePS.hlsl");

	if( m_enableEffects ) {
		m_composeShader = m_shaderFactory->createDeferredComposeShader(
			L"Shaders/Game/deferredCompose.hlsl");
	} else {
		m_composeShader = m_shaderFactory->createDeferredComposeShader(
			L"Shaders/Game/deferredComposeSimple.hlsl");
	}

	m_lightShader = m_shaderFactory->createLightShader(
		L"Shaders/Game/lighting.hlsl");
}

void DeferredRenderer::initFullScreenQuad()
{
	m_fullscreenQuad = m_bufferFactory->createFullScreenQuadBuffer();
}

void DeferredRenderer::initSSAO()
{
	m_ssaoData.scale	= 4.0f;
	m_ssaoData.bias		= 0.242f;
	m_ssaoData.intensity= 1.0f;
	m_ssaoData.sampleRadius=0.02f;
	m_ssaoData.epsilon  = 0.0f;
	m_ssaoData.cocFactor  = 1.0f;
}

void DeferredRenderer::unmapAllBuffers()
{
	//ID3D11ShaderResourceView* nulz[RenderTargets_CNT];
	//for (int i=0; i<RenderTargets_CNT; i++)
	//	nulz[i]=NULL;
	//m_deviceContext->PSSetShaderResources(0,RenderTargets_CNT,nulz);

	unmapGbuffers();
	unmapDofBuffers();
	unmapDepth();

	m_lightShader->apply();
}

void DeferredRenderer::checkHr( HRESULT p_hr, const string& p_file,
							   const string& p_function, int p_line )
{
	if ( p_hr != S_OK ) {
		throw D3DException( p_hr, p_file, p_function, p_line );
	}
}

void DeferredRenderer::createSrvAndRtv( ID3D11ShaderResourceView** out_srv,
	ID3D11RenderTargetView** out_rtv, int p_width, int p_height, DXGI_FORMAT p_format )
{
	D3D11_TEXTURE2D_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	int dofBufferFactor = 4;
	bufferDesc.Width = p_width;
	bufferDesc.Height = p_height;
	bufferDesc.MipLevels = 1;
	bufferDesc.ArraySize = 1;
	bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.Format = p_format;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.SampleDesc.Quality = 0;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	HRESULT hr = S_OK;

	ID3D11Texture2D* bufferTexture;
	hr = m_device->CreateTexture2D( &bufferDesc, NULL, &bufferTexture );		
	checkHr( hr, __FILE__, __FUNCTION__, __LINE__ );

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = bufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateRenderTargetView( bufferTexture, &renderTargetViewDesc, out_rtv );
	checkHr( hr, __FILE__, __FUNCTION__, __LINE__ );

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc;
	shaderResourceDesc.Format = bufferDesc.Format;
	shaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceDesc.Texture2D.MipLevels = 1;
	shaderResourceDesc.Texture2D.MostDetailedMip = 0;

	hr = m_device->CreateShaderResourceView( bufferTexture, &shaderResourceDesc, out_srv );
	checkHr( hr, __FILE__, __FUNCTION__, __LINE__ );

	bufferTexture->Release();
}