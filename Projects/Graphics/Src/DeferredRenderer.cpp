#include "AntTweakBarWrapper.h"
#include "BufferFactory.h"
#include "DeferredRenderer.h"
#include "LightMesh.h"
#include "Mesh.h"
#include "ShaderFactory.h"
#include "Texture.h"

#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "GUIShader.h"

#include "PNTVertex.h"
#include "PNTTBVertex.h"
#include <LightInstanceData.h>

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

	initFullScreenQuad();

	buildBlendStates();
	m_currentBlendStateType = BlendState::DEFAULT;

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
	delete m_composeShader;
	delete m_fullscreenQuad;
}

void DeferredRenderer::clearBuffers()
{
	unMapGBuffers();
	float clearColor[] = {
		0.0f,0.0f,0.0f,1.0f
	};
	for (unsigned int i = 0; i < RenderTargets::NUMTARGETS; i++){
		m_deviceContext->ClearRenderTargetView(m_gBuffers[i], clearColor);
	}

	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DeferredRenderer::renderComposeStage()
{
	m_composeShader->apply();
	m_fullscreenQuad->apply();

	m_deviceContext->Draw(6,0);
}
void DeferredRenderer::mapDeferredBaseRTSToShader()
{	
	m_deviceContext->PSSetShaderResources( 0, 3, m_gBuffersShaderResource);
	m_deviceContext->PSSetShaderResources( 3, 1, &m_gBuffersShaderResource[
		RenderTargets::DEPTH] );
}

void DeferredRenderer::mapVariousPassesToComposeStage(){
	m_deviceContext->PSSetShaderResources(0, 1, &m_gBuffersShaderResource[
		RenderTargets::LIGHT] );
}
void DeferredRenderer::unmapVariousPassesFromComposeStage(){
	ID3D11ShaderResourceView* nulz = NULL;
	m_deviceContext->PSSetShaderResources(0, 1, &nulz);
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
		L"Shaders/Game/deferredBase.hlsl");

	m_composeShader = m_shaderFactory->createDeferredComposeShader(
		L"Shaders/Game/deferredCompose.hlsl");

	m_lightShader = m_shaderFactory->createLightShader(
		L"Shaders/Game/lighting.hlsl");
}

void DeferredRenderer::initFullScreenQuad()
{
	m_fullscreenQuad = m_bufferFactory->createFullScreenQuadBuffer();
}

void DeferredRenderer::hookUpAntTweakBar()
{
	AntTweakBarWrapper::getInstance()->addWriteVariable( AntTweakBarWrapper::GRAPHICS,
		"Color",TW_TYPE_COLOR4F, 
		&m_baseShader->getPerFrameBufferPtr()->accessBuffer.color[0], "");
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

DeferredBaseShader* DeferredRenderer::getDeferredLightShader(){
	return m_lightShader;
}
