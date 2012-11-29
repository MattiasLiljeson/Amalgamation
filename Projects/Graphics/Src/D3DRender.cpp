#include "D3DRender.h"

D3DRender::D3DRender(HWND p_hWnd, int p_width, int p_height, bool p_windowed)
{
	m_device		= NULL;
	m_deviceContext = NULL;
	m_swapChain		= NULL;

	//m_depthStencilView = NULL;
	//m_backBuffer	   = NULL;

	m_shader		= NULL;
	m_vertexBuffer	= NULL;

	m_width	= p_width;
	m_height= p_height;

	initHardware(p_hWnd, p_windowed);
	
	m_shader = new Shader(m_device, m_deviceContext);

	initBuffers();
	initViewport();
	initFullScreenQuad();

	m_deferred = new Deferred( m_device, m_deviceContext, m_width, m_height);
	clearRenderTargets();
}

D3DRender::~D3DRender()
{
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_backBuffer);

	delete m_shader;
	delete m_vertexBuffer;
}

void D3DRender::initHardware(HWND p_hWnd, bool p_windowed)
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

void D3DRender::initBuffers()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* backBufferTexture;

	hr = m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), 
		(LPVOID*)&backBufferTexture );

	if( FAILED(hr) )
		throw D3DException("Failed to get backbuffer from swapchain.",__FILE__,
		__FUNCTION__,__LINE__);

	hr = m_device->CreateRenderTargetView( backBufferTexture, NULL, &m_backBuffer );
	backBufferTexture->Release();
	if( FAILED(hr) )
		throw D3DException("Failed to create rendertargetview from backbuffer.",
		__FILE__,__FUNCTION__,__LINE__);
}

void D3DRender::clearRenderTargets()
{
	m_deferred->clearBuffers();
	
	static float ClearColor[4] = { 1, 0, 0, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_backBuffer,ClearColor);
}

void D3DRender::render()
{
	m_deferred->deferredBasePass();

	m_deviceContext->OMSetRenderTargets( 1, &m_backBuffer, NULL);
	m_deferred->renderComposedImage();
	/*
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	UINT32 vertexSize = sizeof(PTVertex);
	UINT32 offset = 0;
	
	m_vertexBuffer->apply();
	m_shader->apply();

	m_deviceContext->Draw(6,0);
	*/
}

void D3DRender::flipBackBuffer()
{
	m_swapChain->Present( 0, 0);
}

void D3DRender::initFullScreenQuad()
{
	PTVertex mesh[]= {
		{{ 1,	-1,	0},	{ 1, 1}},
		{{ -1,	-1,	0},	{ 0, 1}},
		{{ 1,	1,	0},	{ 1, 0}},

		{{ -1, -1,	0},	{ 0, 1}},
		{{ 1,	1,	0},	{ 1, 0}},
		{{ -1,	1,	0},	{ 0, 0}}
	};

	Buffer::BUFFER_INIT_DESC bufferDesc;
	bufferDesc.ElementSize = sizeof(PTVertex);
	bufferDesc.InitData = mesh;
	bufferDesc.NumElements = 6;
	bufferDesc.Type = Buffer::VERTEX_BUFFER;
	bufferDesc.Usage = Buffer::BUFFER_DEFAULT;
	
	m_vertexBuffer = new Buffer(m_device,m_deviceContext,bufferDesc);
}

void D3DRender::initViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)m_width;
	vp.Height = (float)m_height;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_deviceContext->RSSetViewports(1, &vp);
}
