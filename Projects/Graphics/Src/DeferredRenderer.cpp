#include "AntTweakBarWrapper.h"
#include "BufferFactory.h"
#include "DeferredRenderer.h"
#include "ShaderFactory.h"
#include "Texture.h"

#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "DeferredAnimatedBaseShader.h"
#include "LightShader.h"
#include "GUIShader.h"


DeferredRenderer::DeferredRenderer(ID3D11Device* p_device, 
								   ID3D11DeviceContext* p_deviceContext, 
								   int p_width, 
								   int p_height)
{
	m_device		= p_device;
	m_deviceContext = p_deviceContext;

	m_width		= p_width;
	m_height	= p_height;

	m_shaderFactory = new ShaderFactory(m_device,m_deviceContext, 
		m_device->GetFeatureLevel());

	
	m_depthStencilView = NULL;
	for(int i = 0; i < RenderTargets::NUMTARGETS; i++)
	{
		m_gBuffers[i] = NULL;
		m_gBuffersShaderResource[i] = NULL;
	}

	m_bufferFactory = new BufferFactory(m_device,m_deviceContext);


	initRendertargetsAndDepthStencil( m_width, m_height );

	initShaders();
	initSSAO();
	initFullScreenQuad();

	buildBlendStates();
	m_currentBlendStateType = BlendState::DEFAULT;
	m_blendMask = 0xffffffff;
	for (int i=0;i<4;i++) m_blendFactors[i]=1;

	buildRasterizerStates();
	m_currentRasterizerStateType = RasterizerState::DEFAULT;

}

DeferredRenderer::~DeferredRenderer()
{
	releaseRenderTargetsAndDepthStencil();

	for (unsigned int i = 0; i < m_blendStates.size(); i++){
		SAFE_RELEASE(m_blendStates[i]);
	}

	for (unsigned int i = 0; i < m_blendStates.size(); i++){
		SAFE_RELEASE(m_rasterizerStates[i]);
	}

	delete m_shaderFactory;
	delete m_bufferFactory;
	delete m_baseShader;
	delete m_lightShader;
	delete m_ssaoShader;
	delete m_composeShader;
	delete m_fullscreenQuad;
	delete m_animatedBaseShader;
}

void DeferredRenderer::clearBuffers()
{
	unMapGBuffers();
	float clearColor[] = {
		0.0f,0.0f,0.0f,0.0f
	};
	for (unsigned int i = 0; i < RenderTargets::NUMTARGETS; i++){
		m_deviceContext->ClearRenderTargetView(m_gBuffers[i], clearColor);
	}

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeferredRenderer::renderSsao()
{
	m_ssaoShader->setSSAOBufferData(m_ssaoData);
	m_ssaoShader->apply();
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
void DeferredRenderer::mapDeferredBaseRTSToShader(ID3D11ShaderResourceView* p_shadowMap)
{	
	m_deviceContext->PSSetShaderResources( 0, 3, m_gBuffersShaderResource);
	m_deviceContext->PSSetShaderResources( 3, 1, &m_gBuffersShaderResource[
		RenderTargets::DEPTH] );

		m_deviceContext->PSSetShaderResources( 4, 1, &p_shadowMap);
}
void DeferredRenderer::mapDeferredBaseRTSToShader()
{	
	m_deviceContext->PSSetShaderResources( 0, 3, m_gBuffersShaderResource);
	m_deviceContext->PSSetShaderResources( 3, 1, &m_gBuffersShaderResource[
		RenderTargets::DEPTH] );
}

void DeferredRenderer::unmapDeferredBaseFromShader(){
	ID3D11ShaderResourceView* nulz = NULL;
	m_deviceContext->PSSetShaderResources( 4, 1, &nulz);
}
void DeferredRenderer::mapVariousPassesToComposeStage(){
	m_deviceContext->PSSetShaderResources(0, 1, &m_gBuffersShaderResource[
		RenderTargets::LIGHT] );
	m_deviceContext->PSSetShaderResources(1, 1, &m_gBuffersShaderResource[
			RenderTargets::NORMAL] );
	m_deviceContext->PSSetShaderResources(2, 1, &m_gBuffersShaderResource[
		RenderTargets::DEPTH] );
}

void DeferredRenderer::unmapVariousPassesFromComposeStage(){
	ID3D11ShaderResourceView* nulz = NULL;
	m_deviceContext->PSSetShaderResources( 0, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 1, 1, &nulz );
	m_deviceContext->PSSetShaderResources( 2, 1, &nulz );
}
void DeferredRenderer::unMapGBuffers()
{
	ID3D11ShaderResourceView* nulz[NUMTARGETS];
	for (int i=0; i<NUMTARGETS; i++)
		nulz[i]=NULL;
	m_deviceContext->PSSetShaderResources(0,NUMTARGETS,nulz);
	m_lightShader->apply();
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
		&shaderResourceDesc, &m_gBuffersShaderResource[RenderTargets::DEPTH])))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);

	depthStencilTexture->Release();
}

void DeferredRenderer::initGeometryBuffers()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* gBufferTextures[RenderTargets::NUMTARGETS];
	D3D11_TEXTURE2D_DESC gBufferDesc;
	ZeroMemory( &gBufferDesc, sizeof(gBufferDesc) );

	gBufferDesc.Width = m_width;
	gBufferDesc.Height = m_height;
	gBufferDesc.MipLevels = 1;
	gBufferDesc.ArraySize = 1;
	gBufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	gBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	gBufferDesc.SampleDesc.Count = 1;
	gBufferDesc.SampleDesc.Quality = 0;
	gBufferDesc.CPUAccessFlags = 0;
	gBufferDesc.MiscFlags = 0;

	for (unsigned int i = 0; i < RenderTargets::NUMTARGETS; i++){
		hr = m_device->CreateTexture2D(&gBufferDesc,NULL,&gBufferTextures[i]);		
		if (hr != S_OK)
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = gBufferDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	for (unsigned int i = 0; i < RenderTargets::NUMTARGETS; i++){
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
	for (unsigned int i = 0; i < RenderTargets::NUMTARGETS-1; i++){
		hr = m_device->CreateShaderResourceView(gBufferTextures[i],&shaderResourceDesc,
			&m_gBuffersShaderResource[i]);
		gBufferTextures[i]->Release();
		if (hr != S_OK )
			throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
	}
}

void DeferredRenderer::initShaders()
{
	m_baseShader = m_shaderFactory->createDeferredBaseShader(
		L"Shaders/Game/deferredBaseTess.hlsl");

	m_ssaoShader = m_shaderFactory->createDeferredComposeShader(
		L"Shaders/Game/ssaoGenerate.hlsl");
		
	m_animatedBaseShader = m_shaderFactory->createDeferredAnimatedShader(
		L"Shaders/Game/deferredAnimatedBaseTess.hlsl");

	m_composeShader = m_shaderFactory->createDeferredComposeShader(
		L"Shaders/Game/deferredCompose.hlsl");

	m_lightShader = m_shaderFactory->createLightShader(
		L"Shaders/Game/lighting.hlsl");
}

void DeferredRenderer::initFullScreenQuad()
{
	m_fullscreenQuad = m_bufferFactory->createFullScreenQuadBuffer();
}

void DeferredRenderer::hookUpAntTweakBar(){
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
}


void DeferredRenderer::releaseRenderTargetsAndDepthStencil(){
	// Release all buffers
	SAFE_RELEASE(m_depthStencilView);

	for (int i = 0; i < RenderTargets::NUMTARGETS; i++)
	{
		SAFE_RELEASE(m_gBuffers[i]);
		SAFE_RELEASE(m_gBuffersShaderResource[i]);
	}
}

void DeferredRenderer::initRendertargetsAndDepthStencil( int p_width, int p_height ){
	m_width = p_width;
	m_height = p_height;

	initDepthStencil();
	initGeometryBuffers();
}

void DeferredRenderer::setBlendState(BlendState::Mode p_state){
	unsigned int idx = static_cast<unsigned int>(p_state);
	m_deviceContext->OMSetBlendState( m_blendStates[idx], m_blendFactors, m_blendMask );
	m_currentBlendStateType = p_state;
}

void DeferredRenderer::setBlendFactors( float p_red, float p_green, float p_blue, 
									    float p_alpha ){
	m_blendFactors[0]=p_red;
	m_blendFactors[1]=p_green;
	m_blendFactors[2]=p_blue;
	m_blendFactors[3]=p_alpha;
}

void DeferredRenderer::setBlendFactors( float p_oneValue ){
	for (int i=0;i<4;i++)
		m_blendFactors[i]=p_oneValue;
}

void DeferredRenderer::setBlendMask( UINT p_mask ){
	m_blendMask = p_mask;
}

void DeferredRenderer::setRasterizerStateSettings(RasterizerState::Mode p_state){
	unsigned int idx = static_cast<unsigned int>(p_state);
	m_deviceContext->RSSetState( m_rasterizerStates[idx] );
	m_currentRasterizerStateType = p_state;
}

void DeferredRenderer::buildBlendStates(){
	RenderStateHelper::fillBlendStateList(m_device,m_blendStates);
}

void DeferredRenderer::buildRasterizerStates(){
	RenderStateHelper::fillRasterizerStateList(m_device,m_rasterizerStates);
}
void DeferredRenderer::setBasePassRenderTargets(){
	m_deviceContext->OMSetRenderTargets(BASESHADERS,m_gBuffers,m_depthStencilView);
}

ID3D11DepthStencilView* DeferredRenderer::getDepthStencil(){
	return m_depthStencilView;
}

void DeferredRenderer::unmapDepthFromShaderVariables(){
	ID3D11ShaderResourceView* nulz = NULL;
	m_deviceContext->PSSetShaderResources( 3, 1, &nulz );
}

void DeferredRenderer::setLightRenderTarget(){
	m_deviceContext->OMSetRenderTargets(1,&m_gBuffers[RenderTargets::LIGHT],NULL);
}

DeferredBaseShader* DeferredRenderer::getDeferredBaseShader(){
	return m_baseShader;
}
DeferredAnimatedBaseShader* DeferredRenderer::getDeferredAnimatedBaseShader(){
	return m_animatedBaseShader;
}
LightShader* DeferredRenderer::getDeferredLightShader(){
	return m_lightShader;
}

ID3D11ShaderResourceView*const* DeferredRenderer::getShaderResourceView( RenderTargets p_target )
{
	return &m_gBuffersShaderResource[p_target];
}

void DeferredRenderer::initSSAO()
{
	m_ssaoData.scale	= 4.0f;
	m_ssaoData.bias		= 0.242f;
	m_ssaoData.intensity= 1.0f;
	m_ssaoData.sampleRadius=0.02f;
	m_ssaoData.epsilon  = 0.0f;
}