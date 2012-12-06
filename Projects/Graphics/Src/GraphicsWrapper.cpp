#include "GraphicsWrapper.h"

GraphicsWrapper::GraphicsWrapper(HWND p_hWnd, int p_width, int p_height, bool p_windowed)
{
	m_device		= NULL;
	m_deviceContext = NULL;
	m_swapChain		= NULL;

	m_deferredBaseShader	= NULL;

	m_width	= p_width;
	m_height= p_height;

	initHardware(p_hWnd, p_windowed);

	initBackBuffer();
	initViewport();

	m_bufferFactory = new BufferFactory(m_device,m_deviceContext);
	m_meshManager = new ResourceManager<Mesh>();
	m_textureManager = new ResourceManager<Texture>();

	m_deferredRenderer = new DeferredRenderer( m_device, m_deviceContext, 
							   m_width, m_height);



	clearRenderTargets();
}

GraphicsWrapper::~GraphicsWrapper()
{
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_backBuffer);
	
	delete m_deferredRenderer;
	delete m_deferredBaseShader;
	delete m_bufferFactory;
	delete m_meshManager;
	delete m_textureManager;
}

void GraphicsWrapper::initHardware(HWND p_hWnd, bool p_windowed)
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType;

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = p_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = p_windowed;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	int selectedDriverType = -1;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&sd,
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
	
	static float ClearColor[4] = { 1, 0, 0, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_backBuffer,ClearColor);
}

void GraphicsWrapper::setSceneInfo(const RendererSceneInfo& p_sceneInfo)
{
	m_deferredRenderer->setSceneInfo(p_sceneInfo);
}

void GraphicsWrapper::beginFrame()
{
	m_deferredRenderer->beginDeferredBasePass();
}

void GraphicsWrapper::renderMesh(unsigned int p_meshId)
{
	Mesh* mesh = m_meshManager->getResource(p_meshId);
	Texture* tex = m_textureManager->getResource(mesh->getTextureId());
	m_deferredRenderer->renderMesh(mesh,tex);
}

void GraphicsWrapper::finalizeFrame()
{
	m_deviceContext->OMSetRenderTargets( 1, &m_backBuffer, NULL);
	m_deferredRenderer->renderComposedImage();
}

void GraphicsWrapper::flipBackBuffer()
{
	m_swapChain->Present( 0, 0);
}

unsigned int GraphicsWrapper::createMesh(const string& p_name, unsigned int p_ownerEntityId)
{
	// check if resource already exists
	unsigned int meshResultId = 0;
	int meshFoundId = m_meshManager->getResourceId(p_name);
	if (meshFoundId==-1)  // if it does not exist, create new
	{
		if (p_name=="P_cube")
		{
			Mesh* mesh = m_bufferFactory->createBoxMesh(); // construct a mesh
			mesh->addInstanceId(p_ownerEntityId);		   // add owner
			meshResultId = m_meshManager->addResource(p_name,mesh);	   // put in manager
			// (Here you might want to do similar checks for textures/materials
			// For now we have a hard coded texture path, but later on
			// we probably get this path from a mesh file loader or similar.
			string texturepath = "Assets/Textures/Test/1000x1000_32.png"; 
			int texFoundId = m_textureManager->getResourceId(texturepath);
			unsigned int texResultId = 0;
			// and probably only the file name or Texture sub folder name+file name
			// like this: rainbow.dds or Test/rainbow.dds
			if (texFoundId==-1)  // if it does not exist, create new
			{
				Texture* tex;
				tex = new Texture(TextureParser::loadTexture(m_device,
															 texturepath.c_str()));
				texResultId = m_textureManager->addResource(texturepath,tex);
			}
			else
			{
				texResultId = static_cast<unsigned int>(texFoundId);
			}
			mesh->setTextureId(texResultId);
			// and their managers.)
			// ...
			// and then set the resulting data to the mesh
		}
		else
		{
			// load from path instead
		}
	}
	else // the mesh already exists
	{
		meshResultId = static_cast<unsigned int>(meshFoundId);
		Mesh* mesh = m_meshManager->getResource(meshResultId); // get mesh from id
		mesh->addInstanceId(p_ownerEntityId); // add owner
	}
	return meshResultId;
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
