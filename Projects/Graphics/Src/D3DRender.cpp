#include "D3DRender.h"

D3DRender::D3DRender(HWND p_hWnd, int p_width, int p_height, bool p_windowed)
{
	m_device		= NULL;
	m_deviceContext = NULL;
	m_swapChain		= NULL;

	m_depthStencilView = NULL;
	m_backBuffer	   = NULL;

	m_shader		= NULL;
	m_vertexBuffer	= NULL;

	m_width		= p_width;
	m_height	= p_height;

	initHardware(p_hWnd, p_windowed);
	
	m_shader = new Shader(m_device, m_deviceContext);

	initBuffers();
	initViewport();
	initFullScreenQuad();

	clearRenderTargets();
}

D3DRender::~D3DRender()
{
	SAFE_RELEASE(m_device);
	SAFE_RELEASE(m_deviceContext);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_backBuffer);
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
		
		if ( FAILED(hr) )
			throw D3DException("Couldn't create the device and/or swapchain",__FILE__,
			__FUNCTION__,__LINE__);
		else
			break;
	}
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


	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = m_width;
	descDepth.Height = m_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_device->CreateTexture2D( &descDepth, NULL, &m_depthStencil );
	if( FAILED(hr) )
		throw D3DException("Failed to create depth stencil texture2d.",__FILE__,
		__FUNCTION__,__LINE__);


	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_device->CreateDepthStencilView( m_depthStencil, &descDSV, &m_depthStencilView );
	if( FAILED(hr) )
		throw D3DException("Failed to create the depth stencil view.",__FILE__,
		__FUNCTION__,__LINE__);

}

void D3DRender::clearRenderTargets()
{
	static float ClearColor[4] = { 1, 0, 0, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_backBuffer,ClearColor);
	m_deviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_deviceContext->OMSetRenderTargets( 1, &m_backBuffer, m_depthStencilView);
}

void D3DRender::render()
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	UINT32 vertexSize = sizeof(PTVertex);
	UINT32 offset = 0;
	
	m_shader->apply();

	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);

	m_deviceContext->Draw(6,0);

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

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc,sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(PTVertex)*6;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &mesh[0];

	HRESULT hr = S_OK;
	hr = m_device->CreateBuffer(&bufferDesc, &data, &m_vertexBuffer);
	if (FAILED (hr) )
		throw D3DException(hr,__FILE__,__FUNCTION__,__LINE__);
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
