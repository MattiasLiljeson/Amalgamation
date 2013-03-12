#include "Game.h"
#include <AntTweakBar.h>
#include "Mesh.h"
#include "RasterManager.h"
#include "SphereMesh.h"
#include "Globals.h"
#include "BoxMesh.h"
#include "ParticleStates.h"

TwBar *bar;

void TW_CALL OpenMeshDialog(void *clientData)
{
	int i = (int)clientData;
	Mesh* m = Scene::GetInstance()->GetMesh(i);
	m->SetVisible(!m->IsVisible());
}

void TW_CALL LoadAGL(void *clientData)
{
	string file = openfilename();
	if (file != "")
		AGLLoader::GetInstance()->Load(file);

	vector<Mesh*> meshes = Scene::GetInstance()->GetMeshes();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		string s = "Mesh " + toString(i);
		string info = " label='" + s + "' help='Load an Agile file into the editor.' group='Meshes'";
		TwAddButton(bar, s.c_str(), OpenMeshDialog, (void*)i, info.c_str());
	}
}

Game::Game(HINSTANCE pInstanceHandle):
	DX11Application(pInstanceHandle)
{
}

bool Game::Initialize()
{
	DX11Application::Initialize();
	ShaderManager::GetInstance()->Initialize(mDevice, mDeviceContext);
	Camera::GetInstance()->Init(AglVector3(0, 0.0f, -2.25f), AglVector3(0, 0.0f, 0), AglVector3(0, 1, 0), mScreenWidth, mScreenHeight);
	TextureManager::GetInstance()->Init(mDevice, mDeviceContext);
	RasterManager::getInstance()->initialize(mDevice, mDeviceContext);

	ParticleStates::Init(mDevice, mDeviceContext);
	
	AGLLoader::GetInstance()->Init(mDevice, mDeviceContext);

	// Initialize AntTweakBar
    if (!TwInit(TW_DIRECT3D11, mDevice))
    {
        MessageBoxA(mWindowHandle, TwGetLastError(), "AntTweakBar initialization failed", MB_OK|MB_ICONERROR);
        Cleanup();
        return 0;
    }
	SceneDialog::GetInstance();


	m_cameraController = new CameraController(mWindowHandle);

	SPHEREMESH = new SphereMesh(mDevice, mDeviceContext);
	BOXMESH = new BoxMesh(mDevice, mDeviceContext);

	return true;
}
bool Game::Cleanup()
{
	AGLLoader::Release();
	ShaderManager::Release();
	Camera::Release();
	TextureManager::Release();
	Scene::Release();
	DX11Application::Cleanup();
	SceneDialog::Release();
	RasterManager::cleanup();
	TwTerminate();
	delete m_cameraController;
	delete SPHEREMESH;
	delete BOXMESH;
	return true;
}
bool Game::Update(float pElapsedTime)
{
	DX11Application::Update(pElapsedTime);
	/*if(GetAsyncKeyState(VK_F5) & 0x8000)
	{
		TextureManager::GetInstance()->ReloadAll();
	}*/
	if(GetAsyncKeyState(VK_F5) & 0x8000)
	{
			Scene::GetInstance()->Save("");
	}
	else if(GetAsyncKeyState(VK_F6) & 0x8000)
	{
		string file = savefilename();
		if (file != "")
			Scene::GetInstance()->Save(file);
	}

	m_cameraController->Update(pElapsedTime);
	Scene::GetInstance()->Update(pElapsedTime);
	return true;
}
bool Game::Draw(float pElapsedTime)
{
	DX11Application::Draw(pElapsedTime);
    //mDeviceContext->ClearRenderTargetView(mBackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mBackBuffer, color);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	Scene::GetInstance()->Draw();
	   TwDraw();

    mSwapChain->Present(0, 0);
	return true;
}