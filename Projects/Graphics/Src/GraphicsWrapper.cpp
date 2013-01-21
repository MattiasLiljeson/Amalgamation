#include "GraphicsWrapper.h"
#include "DeferredBaseShader.h"
#include "DeferredComposeShader.h"
#include "DeferredRenderer.h"
#include "Mesh.h"
#include "BufferFactory.h"
#include "AntTweakBarWrapper.h"
#include "TextureParser.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshLoadException.h"

#include <AglReader.h>
#include <FileCheck.h>
#include "D3DException.h"
#include "D3DUtil.h"
#include "ParticleRenderer.h"
#include "AglParticleSystem.h"

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

	initSwapChain(p_hWnd);
	initHardware();

	initBackBuffer();
	initViewport();

	m_bufferFactory		= new BufferFactory(m_device,m_deviceContext);
	m_renderSceneInfoBuffer = m_bufferFactory->createRenderSceneInfoCBuffer();
	m_meshManager		= new ResourceManager<Mesh>();
	m_textureManager	= new ResourceManager<Texture>();

	m_textureFactory	= new TextureFactory(m_device,m_textureManager);
	m_modelFactory		= new ModelExtendedManagedFactory(m_device,m_bufferFactory,m_meshManager,
												   m_textureFactory);

	createTexture("mesherror.png",TEXTUREPATH);

	m_deferredRenderer = new DeferredRenderer( m_device, m_deviceContext, 
							   m_width, m_height);
	m_particleRenderer = new ParticleRenderer( m_device, m_deviceContext);

	clearRenderTargets();
}

GraphicsWrapper::~GraphicsWrapper()
{
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_swapChain);
	releaseBackBuffer();
	
	delete m_deferredRenderer;
	delete m_particleRenderer;
	delete m_deferredBaseShader;
	delete m_bufferFactory;
	delete m_meshManager;
	delete m_textureManager;
	delete m_textureFactory;
	delete m_modelFactory;
	delete m_renderSceneInfoBuffer;
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
	
	static float ClearColor[4] = { 1, 0, 0.39f, 1.0f }; //PINK!
	m_deviceContext->ClearRenderTargetView( m_backBuffer,ClearColor);
}

void GraphicsWrapper::updateRenderSceneInfo(const RendererSceneInfo& p_sceneInfo){
	m_renderSceneInfo = p_sceneInfo;
	m_deferredRenderer->setSceneInfo(p_sceneInfo);
}

void GraphicsWrapper::beginFrame()
{
	setRasterizerStateSettings(RasterizerState::DEFAULT);
	m_deferredRenderer->beginDeferredBasePass();

	m_renderSceneInfoBuffer->accessBuffer.setSceneInfo( m_renderSceneInfo );
	m_renderSceneInfoBuffer->update();
	m_renderSceneInfoBuffer->apply(1);

}

void GraphicsWrapper::updatePerFrameConstantBuffer()
{
	m_deferredRenderer->updatePerFrameConstantBuffer();
}

void GraphicsWrapper::renderMesh(unsigned int p_meshId,
								 vector<InstanceData>* p_instanceList)
{
	Mesh* mesh = m_meshManager->getResource(p_meshId);
	
	/************************************************************************/
	/* Get the size of the materials and use it to create a new texture		*/
	/* array.																*/
	/************************************************************************/
	unsigned int arraySize = mesh->getMaterialInfo().SIZE;
	Texture** textureArray = new Texture*[arraySize];
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

	Buffer<InstanceData>* instanceBuffer;
	instanceBuffer = m_bufferFactory->createInstanceBuffer(&(*p_instanceList)[0],
														   p_instanceList->size());
	m_deferredRenderer->renderMeshInstanced(mesh, textureArray, arraySize, instanceBuffer);

	delete textureArray;
	delete instanceBuffer;
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


void GraphicsWrapper::setScissorRegion( int x, int y, int width, int height )
{
	D3D11_RECT scissor_rect;
	scissor_rect.left = x;
	scissor_rect.right = x + width;
	scissor_rect.top = y;
	scissor_rect.bottom = y + height;

	m_deviceContext->RSSetScissorRects(1, &scissor_rect);
}

void GraphicsWrapper::beginGUIPass()
{

	m_deferredRenderer->beginGUIPass();
}

void GraphicsWrapper::renderGUIMesh( unsigned int p_meshId, 
									 vector<InstanceData>* p_instanceList )
{
	Mesh* mesh = m_meshManager->getResource( p_meshId );
	Texture* tex = m_textureManager->getResource( 
		mesh->getMaterialInfo().getTextureType(MaterialInfo::DIFFUSEMAP) );

	Buffer<InstanceData>* instanceBuffer;
	instanceBuffer = m_bufferFactory->createInstanceBuffer( &(*p_instanceList)[0],
		p_instanceList->size() );

	m_deferredRenderer->renderGUIMesh( mesh, tex );

	delete instanceBuffer;
}

void GraphicsWrapper::finalizeGUIPass()
{
	m_deferredRenderer->finalizeGUIPass();
}

void GraphicsWrapper::beginLightPass()
{
	//setRasterizerStateSettings( RasterizerState::FILLED_CCW, false );
	setRasterizerStateSettings( RasterizerState::FILLED_CW_FRONTCULL, false );
	m_deviceContext->OMSetRenderTargets( 1, &m_backBuffer, NULL );
	m_deferredRenderer->beginLightPass();
}

void GraphicsWrapper::renderLights( LightMesh* p_mesh,
								   vector<InstanceData>* p_instanceList )
{
	if( p_mesh != NULL && p_instanceList != NULL )
	{
		Buffer<InstanceData>* instanceBuffer;
		instanceBuffer = m_bufferFactory->createInstanceBuffer( &(*p_instanceList)[0],
			p_instanceList->size() );

		m_deferredRenderer->renderLights( p_mesh, instanceBuffer );

		delete instanceBuffer;
		instanceBuffer = NULL;
	}
	else
	{
		m_deferredRenderer->renderLights( NULL, NULL );
	}
}

void GraphicsWrapper::endLightPass()
{
	m_deferredRenderer->endLightPass();
}


void GraphicsWrapper::flipBackBuffer()
{
	m_swapChain->Present( 0, 0);
}

ModelResource* GraphicsWrapper::createModelFromFile(const string& p_name,
						   const string* p_path)
{
	return m_modelFactory->createModelResource(p_name,p_path);
}

vector<ModelResource*>* GraphicsWrapper::createModelsFromFile(const string& p_name,
									 const string* p_path)
{
	return m_modelFactory->createModelResources(p_name,p_path);
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
	int p_height, int p_pitch, TextureParser::TEXTURE_TYPE p_type )
{
	return m_textureFactory->createTexture(p_source,p_width,p_height,p_pitch,p_type);
}

int GraphicsWrapper::getMeshId( const string& p_name )
{
	return m_meshManager->getResourceId(p_name);
}

void GraphicsWrapper::initViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width	= (float)m_width;
	vp.Height	= (float)m_height;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_deviceContext->RSSetViewports(1, &vp);
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
	initViewport();

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

void GraphicsWrapper::beginParticleRender(){
	beginGUIPass();
}
void GraphicsWrapper::renderParticleSystem( AglParticleSystem* p_system ){
	m_particleRenderer->renderParticles(p_system, m_renderSceneInfo);
}
void GraphicsWrapper::endParticleRender(){
}
