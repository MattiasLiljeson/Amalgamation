#ifndef DX11APPLICATION_H
#define DX11APPLICATION_H

#include "Utility.h"
#include <Windows.h>

class Editor;

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
	ID3D11RasterizerState*		mRasterState;

	int						mScreenWidth;
	int						mScreenHeight;

public:
	DX11Application(HINSTANCE pInstanceHandle);
	virtual bool Initialize();
	virtual bool Cleanup();
	void Run();
	virtual bool Update(float pElapsedTime);
	virtual bool Draw(float pElapsedTime);

	static float dt;
};

#endif