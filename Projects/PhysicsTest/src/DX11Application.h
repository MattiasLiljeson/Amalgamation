#ifndef DX11APPLICATION_H
#define DX11APPLICATION_H

#include "Utility.h"

class InputLayoutManager;
class ShaderManager;
class TestTesselatedTriangle;
class TestTriangle;
class CameraController;

class DX11Application
{
protected:
	IDXGISwapChain*			mSwapChain;
	ID3D11Device*			mDevice;
	ID3D11DeviceContext*	mDeviceContext; 
	HWND					mWindowHandle;
	HINSTANCE				mInstanceHandle;
	Timer					mTimer;

	ID3D11Texture2D*			mDepthStencilBuffer;
	ID3D11RenderTargetView*		mBackBuffer;
	ID3D11DepthStencilView*		mDepthStencilView;
	ID3D11DepthStencilState*	mDepthStencilState;

	int						mScreenWidth;
	int						mScreenHeight;

public:
	DX11Application(HINSTANCE pInstanceHandle);
	virtual bool Initialize();
	virtual bool Cleanup();
	bool Run();
	virtual bool Update(float pElapsedTime);
	virtual bool Draw(float pElapsedTime);

	static bool hitbug;
	
	static float t1;
	static float t2;
	static float t3;
	static ID3D11RasterizerState*	mRasterState;
	static ID3D11RasterizerState*   mWireframeState;
};

#endif