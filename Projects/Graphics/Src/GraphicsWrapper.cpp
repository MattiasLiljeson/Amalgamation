#include "AglParticleSystem.h"
#include "AntTweakBarWrapper.h"
#include "BufferFactory.h"
#include "D3DException.h"
#include "D3DUtil.h"
#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "DeferredRenderer.h"
#include "GraphicsWrapper.h"
#include "Mesh.h"
#include "Mesh.h"
#include "MeshLoadException.h"
#include "ParticleRenderer.h"
#include "Texture.h"
#include "TextureParser.h"
#include "TextureFactory.h"
#include <AglReader.h>
#include <FileCheck.h>
#include <LightInstanceData.h>
#include "ShaderFactory.h"
#include "GUIShader.h"
#include "LightMesh.h"
#include "DeferredBaseShader.h"
#include "ShadowMapRenderer.h"
#include "ShadowShader.h"

GraphicsWrapper::GraphicsWrapper(HWND p_hWnd, int p_width, int p_height, bool p_windowed)
{
	m_device		= NULL;
	m_deviceContext = NULL;
	m_swapChain		= NULL;

	m_deferredBaseShader	= NULL;

	m_width	= p_width;
	m_height= p_height;
	m_windowed = p_windowed;
	m_wireframeMode = false;
	m_renderingShadows = false;

	initSwapChain(p_hWnd);
	initHardware();

	initBackBuffer();

	resetViewportToOriginalSize();

	m_shaderFactory		= new ShaderFactory(m_device, m_deviceContext, 
		m_device->GetFeatureLevel());
	m_bufferFactory		= new BufferFactory(m_device,m_deviceContext);
	m_renderSceneInfoBuffer = m_bufferFactory->createRenderSceneInfoCBuffer();
	m_perShadowBuffer = m_bufferFactory->createPerShadowBuffer();

	m_meshManager		= new ResourceManager<Mesh>();
	m_textureManager	= new ResourceManager<Texture>();

	m_textureFactory	= new TextureFactory(m_device,m_deviceContext,m_textureManager);
	m_modelFactory		= new ModelExtendedFactory(m_device,m_bufferFactory,m_meshManager,
												   m_textureFactory);

	m_guiShader = m_shaderFactory->createGUIShader(
		L"Shaders/GUI/rocket.hlsl");

	m_shadowShader = m_shaderFactory->createShadowShader(
		L"Shaders/Game/shadowMap.hlsl");

	createTexture("mesherror.png",TEXTUREPATH);

	m_deferredRenderer = new DeferredRenderer( m_device, m_deviceContext, 
							   m_width, m_height);
	m_particleRenderer = new ParticleRenderer( m_device, m_deviceContext);

	m_shadowMapRenderer = new ShadowMapRenderer(m_device, m_deviceContext, m_shaderFactory);

	clearRenderTargets();
}

GraphicsWrapper::~GraphicsWrapper()
{
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_swapChain);
	releaseBackBuffer();
	
	delete m_guiShader;
	delete m_shadowShader;
	delete m_deferredRenderer;
	delete m_particleRenderer;
	delete m_shadowMapRenderer;
	delete m_deferredBaseShader;
	delete m_shaderFactory;
	delete m_bufferFactory;
	delete m_meshManager;
	delete m_textureManager;
	delete m_textureFactory;
	delete m_modelFactory;
	delete m_renderSceneInfoBuffer;
	delete m_perShadowBuffer;
}

void GraphicsWrapper::initSwapChain(HWND p_hWnd)
{
	ZeroMemory( &m_swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC) );
	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = m_width;
	m_swapChainDesc.BufferDesc.Height = m_height;
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swapChainDesc.OutputWindow = p_hWnd;
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;
	m_swapChainDesc.Windowed = m_windowed;
}

void GraphicsWrapper::initHardware()
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	int selectedDriverType = -1;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		D3D_DRIVER_TYPE driverType;
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&m_swapChainDesc,
			&m_swapChain,
			&m_device,
			&initiatedFeatureLevel,
			&m_deviceContext);
		
		if (hr == S_OK)
		{
			selectedDriverType = driverTypeIndex;
			break;
		}
	}
	if ( selectedDriverType > 0 )
		throw D3DException("Couldn't create a D3D Hardware-device, software render enabled."
		,__FILE__, __FUNCTION__, __LINE__);
}

void GraphicsWrapper::initBackBuffer()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* backBufferTexture;

	hr = m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), 
		(LPVOID*)&backBufferTexture );

	if( FAILED(hr) )
		throw D3DException("Failed to get backbuffer from swap chain.",__FILE__,
		__FUNCTION__,__LINE__);

	hr = m_device->CreateRenderTargetView( backBufferTexture, NULL, &m_backBuffer );
	backBufferTexture->Release();
	if( FAILED(hr) )
		throw D3DException("Failed to create rendertargetview from back buffer.",
		__FILE__,__FUNCTION__,__LINE__);
}

void GraphicsWrapper::clearRenderTargets()
{
	m_deferredRenderer->clearBuffers();
	
	static float clearColor[4] = { 0.0, 0.0, 0.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_backBuffer,clearColor);
}

void GraphicsWrapper::updateRenderSceneInfo(const RendererSceneInfo& p_sceneInfo){
	m_renderSceneInfo = p_sceneInfo;
}

void GraphicsWrapper::mapSceneInfo(){
	m_renderSceneInfoBuffer->accessBuffer.setSceneInfo( m_renderSceneInfo );
	m_renderSceneInfoBuffer->update();
	m_renderSceneInfoBuffer->apply();
}

void GraphicsWrapper::setActiveShadow( int p_activeShadow ){
	m_perShadowBuffer->accessBuffer.currentShadow = p_activeShadow;
	m_perShadowBuffer->update();
	m_perShadowBuffer->apply();
}
void GraphicsWrapper::unmapPerShadowBuffer()
{
	m_perShadowBuffer->unApply();
}

void GraphicsWrapper::renderMesh(unsigned int p_meshId,
								 vector<InstanceData>* p_instanceList){
	Mesh* mesh = m_meshManager->getResource(p_meshId);

	unsigned int arraySize = 0;
	Texture** textureArray=NULL;

	if(!m_renderingShadows){
		arraySize = mesh->getMaterialInfo().SIZE;
		textureArray = new Texture*[arraySize];
		for (unsigned int i = 0; i < arraySize; i++)
		{
			unsigned int textureId;
			textureId = mesh->getMaterialInfo().getTextureType((MaterialInfo::TextureTypes)i);

			/************************************************************************/
			/* Check if the texture ID is active and get the texture resource or	*/
			/* set the value in the texture array to NULL							*/
			/************************************************************************/
			if(textureId != 0)
				textureArray[i] = m_textureManager->getResource(textureId);
			else
				textureArray[i] = NULL;
		}
	}

	Buffer<InstanceData>* instanceBuffer;
	instanceBuffer = m_bufferFactory->createInstanceBuffer(&(*p_instanceList)[0],
														   p_instanceList->size());
	renderMeshInstanced( 
		mesh->getVertexBuffer()->getBufferPointer(),
		mesh->getVertexBuffer()->getElementSize(),
		mesh->getIndexBuffer()->getBufferPointer(),
		mesh->getIndexBuffer()->getElementCount(),
		textureArray, arraySize,
		instanceBuffer->getElementSize(), 
		instanceBuffer->getBufferPointer(),
		m_deferredRenderer->getDeferredBaseShader());

	delete [] textureArray;
	delete [] instanceBuffer;
}
void GraphicsWrapper::renderLights( LightMesh* p_mesh,
								   vector<LightInstanceData>* p_instanceList )
{
	Buffer<LightInstanceData>* instanceBuffer;
	instanceBuffer = m_bufferFactory->createLightInstanceBuffer( &(*p_instanceList)[0],
		p_instanceList->size() );

	renderMeshInstanced( 
		p_mesh->getVertexBuffer()->getBufferPointer(),
		p_mesh->getVertexBuffer()->getElementSize(),
		p_mesh->getIndexBuffer()->getBufferPointer(),
		p_mesh->getIndexBuffer()->getElementCount(), 
		NULL, 0,
		instanceBuffer->getElementSize(),
		instanceBuffer->getBufferPointer(),
		reinterpret_cast<ShaderBase*>(m_deferredRenderer->getDeferredLightShader()));

	delete instanceBuffer;
	instanceBuffer = NULL;
}

void GraphicsWrapper::setRasterizerStateSettings(RasterizerState::Mode p_state, 
												 bool p_allowWireframeModeOverride)
{
	RasterizerState::Mode state = m_deferredRenderer->getCurrentRasterizerStateType();
	// accept rasterizer state change if not in wireframe mode or 
	// if set to not allow wireframe mode
	if (!m_wireframeMode || !p_allowWireframeModeOverride)
		m_deferredRenderer->setRasterizerStateSettings(p_state);
	else if (state != RasterizerState::WIREFRAME) 
	{   
		// otherwise, force wireframe(if not already set)
		m_deferredRenderer->setRasterizerStateSettings(RasterizerState::WIREFRAME_NOCULL);
	}
}

void GraphicsWrapper::setBlendStateSettings( BlendState::Mode p_state ){
	m_deferredRenderer->setBlendState( p_state );
}

void GraphicsWrapper::setScissorRegion( int x, int y, int width, int height )
{
	D3D11_RECT scissor_rect;
	scissor_rect.left = x;
	scissor_rect.right = x + width;
	scissor_rect.top = y;
	scissor_rect.bottom = y + height;

	m_deviceContext->RSSetScissorRects(1, &scissor_rect);
}

void GraphicsWrapper::setPrimitiveTopology( PrimitiveTopology::Mode p_state ){
	m_deviceContext->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(p_state));
}
void GraphicsWrapper::setBaseRenderTargets(){
	m_deferredRenderer->setBasePassRenderTargets();
}
void GraphicsWrapper::setComposedRenderTargetWithNoDepthStencil(){
	m_deviceContext->OMSetRenderTargets( 1, &m_backBuffer, NULL );
}
void GraphicsWrapper::setLightPassRenderTarget(){
	m_deferredRenderer->setLightRenderTarget();
}
void GraphicsWrapper::setShadowViewProjection( const AglMatrix& p_viewProj ){
	m_shadowShader->sendViewProjection(p_viewProj);
	m_shadowShader->apply();
}

void GraphicsWrapper::setShadowViewProjections( AglMatrix* p_viewProj ){
	m_shadowShader->sendViewProjections(p_viewProj);
	m_shadowShader->apply();
}

void GraphicsWrapper::mapNeededShaderResourceToLightPass( int* p_activeShadows ){
	m_deferredRenderer->mapDeferredBaseRTSToShader();
	int startSlot = 4;
	for(int i = 0; i < MAXSHADOWS; i++){
		if(p_activeShadows[i] != -1){
			m_deviceContext->PSSetShaderResources( startSlot, 1, m_shadowMapRenderer->getShadowMap(i));
		}
	}
}

void GraphicsWrapper::unmapDeferredBaseFromShader(){
	m_deferredRenderer->unmapDeferredBaseFromShader();
}

void GraphicsWrapper::unmapUsedShaderResourceFromLightPass( int* p_activeShadows ){

	ID3D11ShaderResourceView* nulz = NULL;
	int startSlot = 4;
	for(int i = 0; i < MAXSHADOWS; i++){
		if(p_activeShadows[i] != -1){
			m_deviceContext->PSSetShaderResources( startSlot, 1, &nulz);
		}
	}
}

void GraphicsWrapper::renderGUIMeshList( unsigned int p_meshId, 
									 vector<InstanceData>* p_instanceList )
{
	Mesh* mesh = m_meshManager->getResource( p_meshId );
	Texture* tex = m_textureManager->getResource( 
		mesh->getMaterialInfo().getTextureType(MaterialInfo::DIFFUSEMAP) );

	Buffer<InstanceData>* instanceBuffer;
	instanceBuffer = m_bufferFactory->createInstanceBuffer( &(*p_instanceList)[0],
		p_instanceList->size() );

	m_guiShader->apply();
	renderSingleGUIMesh(mesh,tex);

	delete instanceBuffer;
}


void GraphicsWrapper::flipBackBuffer()
{
	m_swapChain->Present( 0, 0);
}

// ModelResource* GraphicsWrapper::createModelFromFile(const string& p_name,
// 						   const string* p_path,bool p_isPrimitive)
// {
// 	return m_modelFactory->createModelResource(p_name,p_path,p_isPrimitive);
// }

vector<ModelResource*>* GraphicsWrapper::createModelsFromFile(const string& p_name,
									 const string* p_path,bool p_isPrimitive)
{
	return m_modelFactory->createModelResources(p_name,p_path,p_isPrimitive);
}

unsigned int GraphicsWrapper::createMeshFromRaw( const string& p_name, 
												int p_numVertices, PNTTBVertex* p_vertices, 
												int p_numIndices, DIndex* p_indices, 
												int p_textureId )
{
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		Mesh* mesh = m_bufferFactory->createMeshFromPNTTBVerticesAndIndices( p_numVertices,
			p_vertices, p_numIndices, p_indices );
		meshResultId = (int)m_meshManager->addResource( p_name, mesh );

		if( p_textureId != -1 )
		{
			MaterialInfo materialInfo;
			materialInfo.setTextureId( MaterialInfo::DIFFUSEMAP, p_textureId);
			mesh->setMaterial( materialInfo );
		}

	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
	}
	return meshResultId;
}


unsigned int GraphicsWrapper::createTexture( const string& p_name, 
											 const string& p_path)
{
	return m_textureFactory->createTexture(p_name,p_path);
}

unsigned int GraphicsWrapper::createTexture( const byte* p_source, int p_width,
	int p_height, int p_pitch, int p_bitLevel, TextureParser::TEXTURE_TYPE p_type )
{
	return m_textureFactory->createTexture(p_source,p_width,p_height,p_pitch, p_bitLevel,
		p_type);
}

int GraphicsWrapper::getMeshId( const string& p_name )
{
	return m_meshManager->getResourceId(p_name);
}

ID3D11Device* GraphicsWrapper::getDevice()
{
	return m_device;
}

ID3D11DeviceContext* GraphicsWrapper::getDeviceContext()
{
	return m_deviceContext;
}

void GraphicsWrapper::hookUpAntTweakBar()
{
	m_deferredRenderer->hookUpAntTweakBar();
}

int GraphicsWrapper::getWindowWidth()
{
	return m_width;
}

int GraphicsWrapper::getWindowHeight()
{
	return m_height;
}

pair<int,int> GraphicsWrapper::getScreenPixelPosFromNDC( float p_x, float p_y )
{
	return getScreenPixelPosFromNDC(p_x, p_y, m_width, m_height);
}

pair<int,int> GraphicsWrapper::getScreenPixelPosFromNDC( float p_x, float p_y,
														int p_width, int p_height )
{
	// NDC -> Screenspace
	int screenSpaceX = static_cast<int>((p_x+1.0f)/2 * (p_width));
	int screenSpaceY = static_cast<int>((p_y+1.0f)/2 * (p_height));
	return pair<int,int>(screenSpaceX,screenSpaceY);
}

pair<float,float> GraphicsWrapper::getNDCPosFromScreenPixel( int p_x, int p_y )
{
	// Screenspace -> NDC
	float ndcX = 2.0f * p_x / m_width - 1.0f;
	float ndcY = 2.0f * p_y / m_height - 1.0f;;
	return pair<float,float>(ndcX, ndcY);
}


void GraphicsWrapper::changeBackbufferRes( int p_width, int p_height ){
	m_width = p_width;
	m_height = p_height;

	m_deviceContext->OMSetRenderTargets(0, 0, 0);

	releaseBackBuffer();
	m_deferredRenderer->releaseRenderTargetsAndDepthStencil();

	HRESULT hr;
	// Resize swap chain to window's size.
	hr = m_swapChain->ResizeBuffers(0, p_width, p_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if(FAILED(hr))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);

	initBackBuffer();
	resetViewportToOriginalSize();

	m_deferredRenderer->initRendertargetsAndDepthStencil( m_width, m_height );
}

void GraphicsWrapper::releaseBackBuffer()
{
	SAFE_RELEASE( m_backBuffer );
}
void GraphicsWrapper::changeToWindowed( bool p_windowed )
{
	HRESULT hr = S_OK;
	m_windowed = !p_windowed;
	hr = m_swapChain->SetFullscreenState((BOOL)m_windowed,nullptr);
	if( FAILED(hr))
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
}

void GraphicsWrapper::setWireframeMode( bool p_wireframe )
{
	m_wireframeMode = p_wireframe;
}

void GraphicsWrapper::renderParticleSystem( AglParticleSystem* p_system ){
	m_particleRenderer->renderParticles(p_system, m_renderSceneInfo);
}

void GraphicsWrapper::setParticleRenderState()
{
	m_deviceContext->OMSetRenderTargets(1,&m_backBuffer,m_deferredRenderer->getDepthStencil());
}

void GraphicsWrapper::unmapDepthFromShader(){
	m_deferredRenderer->unmapDepthFromShaderVariables();
}

void GraphicsWrapper::renderComposeStage(){
	m_deferredRenderer->renderComposeStage();
}

void GraphicsWrapper::mapVariousStagesForCompose(){
	m_deferredRenderer->mapVariousPassesToComposeStage();
}

void GraphicsWrapper::unmapVariousStagesForCompose(){
	m_deferredRenderer->unmapVariousPassesFromComposeStage();
}

void GraphicsWrapper::renderSingleGUIMesh( Mesh* p_mesh, Texture* p_texture )
{
	p_mesh->getVertexBuffer()->apply();
	p_mesh->getIndexBuffer()->apply();

	// set texture
	//HACK: fix so that a placeholder tex is used instead of the last working one
	if( p_texture != NULL ){
		m_deviceContext->PSSetShaderResources(0,1,&(p_texture->data));
	}
	// Draw instanced data
	m_deviceContext->DrawIndexed(p_mesh->getIndexBuffer()->getElementCount(),0,0);
}

void GraphicsWrapper::renderMeshInstanced( void* p_vertexBufferRef, UINT32 p_vertexSize, 
										  void* p_indexBufferRef, UINT32 p_elmentCount, 
										  Texture** p_textureArray, 
										  unsigned int p_textureArraySize, 
										  UINT32 p_instanceDataSize, void* p_instanceRef,
										  ShaderBase* p_shader)
{

	if(p_textureArray){
		UINT startSlot = 0;
		UINT numViews = 1;
		for (unsigned int i = 0; i < p_textureArraySize; i++)
		{
			if(p_textureArray[i] != NULL)
			{
				// set textures
				m_deviceContext->PSSetShaderResources(startSlot , numViews, 
					&p_textureArray[i]->data );
				startSlot++;
			}
		}
	}

	UINT strides[2] = { p_vertexSize, p_instanceDataSize };
	UINT offsets[2] = { 0, 0 };
	// Set up an array of the buffers for the vertices
	ID3D11Buffer* buffers[2] = { 
		static_cast<ID3D11Buffer*>(p_vertexBufferRef), 
		static_cast<ID3D11Buffer*>(p_instanceRef) 
	};

	// Set array of buffers to context 
	m_deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	// And the index buffer
	m_deviceContext->IASetIndexBuffer(static_cast<ID3D11Buffer*>(p_indexBufferRef), 
		DXGI_FORMAT_R32_UINT, 0);

	if(!m_renderingShadows)
		p_shader->apply();

	// Draw instanced data
	m_deviceContext->DrawIndexedInstanced( p_elmentCount, p_instanceDataSize, 0,0,0);
}

void GraphicsWrapper::setViewportToShadowMapSize(){
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width	= static_cast<float>(ShadowMapRenderer::SHADOWMAPSIZE);
	vp.Height	= static_cast<float>(ShadowMapRenderer::SHADOWMAPSIZE);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1,&vp);
}

void GraphicsWrapper::resetViewportToOriginalSize(){
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width	= static_cast<float>(m_width);
	vp.Height	= static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1,&vp);
}

void GraphicsWrapper::setShadowMapAsRenderTarget( unsigned int p_shadowMapIdx ){
	m_shadowMapRenderer->mapShadowMapToRenderTarget(p_shadowMapIdx);
}

void GraphicsWrapper::setRenderingShadows(){
	m_renderingShadows = true;
}

void GraphicsWrapper::stopedRenderingShadows(){
	m_renderingShadows = false;
}

unsigned int GraphicsWrapper::generateShadowMap()
{
	return m_shadowMapRenderer->createANewShadowMap();
}

