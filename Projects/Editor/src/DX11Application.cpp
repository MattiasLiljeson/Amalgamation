#include "DX11Application.h"
#include <AntTweakBar.h>
#include "Globals.h"

float DX11Application::dt = 0;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Send event message to AntTweakBar
    if (TwEventWin(hWnd, message, wParam, lParam))
        return 0; // Event has been handled by AntTweakBar

	float newPos;
    switch(message)
    {
		case WM_KEYDOWN:
		{
			if( wParam == VK_ESCAPE )
				DestroyWindow(hWnd);
			return 0;
		}
		case WM_MOUSEWHEEL:
			newPos = GET_WHEEL_DELTA_WPARAM(wParam);
			MOUSEWHEELDELTA = newPos;/* / -WHEEL_DELTA*/;
		break;
        case WM_DESTROY:
        {
                PostQuitMessage(0);
                return 0;
        } 
		break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

DX11Application::DX11Application(HINSTANCE pInstanceHandle)
{
	mInstanceHandle			= pInstanceHandle;
	mSwapChain				= NULL;
	mDevice					= NULL;
	mDeviceContext			= NULL; 
	mWindowHandle			= 0;
	mScreenWidth			= 1920;
	mScreenHeight			= 1080;
}
bool DX11Application::Initialize()
{
    WNDCLASSEXW wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = mInstanceHandle;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";

    RegisterClassExW(&wc);
	int argc = 0;

    RECT wr = {0, 0, mScreenWidth, mScreenHeight};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    mWindowHandle = CreateWindowExW(NULL,
                          L"WindowClass",
                          L"DX11 Template",
                          WS_OVERLAPPEDWINDOW,
                          0,
                          0,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          mInstanceHandle,
                          NULL);

    ShowWindow(mWindowHandle, SW_SHOW);

    DXGI_SWAP_CHAIN_DESC scd;

    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount					= 1;
    scd.BufferDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width			= mScreenWidth;
	scd.BufferDesc.Height			= mScreenHeight;
    scd.BufferUsage					= DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow				= mWindowHandle;
    scd.SampleDesc.Count			= 1;
	scd.SampleDesc.Quality			= 0;
    scd.Windowed					= TRUE;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SwapEffect					= DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags						= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  featureLevels,
                                  3,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &mSwapChain,
                                  &mDevice,
                                  NULL,
                                  &mDeviceContext);

	ID3D11Texture2D *BackBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	mDevice->CreateRenderTargetView(BackBuffer, NULL, &mBackBuffer);
    BackBuffer->Release();


	// Create the depth/stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width     = mScreenWidth;
	depthBufferDesc.Height    = mScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count   = 1; // multisampling must match
	depthBufferDesc.SampleDesc.Quality = 0; // swap chain values.
	depthBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0; 
	depthBufferDesc.MiscFlags      = 0;

	int result = mDevice->CreateTexture2D(&depthBufferDesc, 0, &mDepthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if(FAILED(result))
	{
		return false;
	}
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
	//mDeviceContext->OMSetDepthStencilState(0, 0);
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if(FAILED(result))
	{
		return false;
	}

    mDeviceContext->OMSetRenderTargets(1, &mBackBuffer, mDepthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if(FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	mDeviceContext->RSSetState(mRasterState);



    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)mScreenWidth;
    viewport.Height = (float)mScreenHeight;
	viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    mDeviceContext->RSSetViewports(1, &viewport);

	mTimer.Reset();

	return true;
}
bool DX11Application::Cleanup()
{
	mSwapChain->SetFullscreenState(FALSE, NULL);
	mSwapChain->Release();
    mDevice->Release();
    mDeviceContext->Release();
	mBackBuffer->Release();
	mDepthStencilView->Release();
	mDepthStencilState->Release();
	mRasterState->Release();
	return true;
}
void DX11Application::Run()
{
	while (true)
	{
		MSG msg;
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT)
				break;
		}
		else
		{
			mTimer.Tick();

			float FPS = 1.0f / mTimer.DeltaTime();
			stringstream ss;
			ss << FPS;
			string s = ss.str();
			s = "FPS: " + s;
			wstring s2(s.length(), L' ');
			
			copy(s.begin(), s.end(), s2.begin());

			SetWindowTextA(mWindowHandle, s.c_str());

			//if (GUI::GetInstance()->GetMainWindow())
				//GUI::GetInstance()->GetMainWindow()->SetWindowTitle(s);

			Update(mTimer.DeltaTime());
			Draw(mTimer.DeltaTime());
			MOUSEWHEELDELTA = 0;
		}
	}
}
bool DX11Application::Update(float pElapsedTime)
{
	DX11Application::dt = pElapsedTime;
	return true;
}
bool DX11Application::Draw(float pElapsedTime)
{
	return true;
}