#include "Game.h"
#include "TestTesselatedTriangle.h"
#include "TestTriangle.h"
#include "CameraController.h"
#include "ShaderManager.h"
#include "GJKSolver.h"
#include "Eigen.h"
#include "AglReader.h"

float Game::Time = 0;

Game::Game(HINSTANCE pInstanceHandle):
	DX11Application(pInstanceHandle)
{
	mTestFloat = 0;
	Avatar = NULL;
	mPhysics = NULL;
}

bool Game::Initialize()
{
	DX11Application::Initialize();
	ShaderManager::GetInstance()->Initialize(mDevice, mDeviceContext);
	Camera::GetInstance()->Init(AglVector3(0, 0, -10), AglVector3(0, 0, 0), AglVector3(0, 1, 0), mScreenWidth, mScreenHeight);
	mCameraController = new CameraController(mWindowHandle);
	//mTestTriangle = new TestTesselatedTriangle(mDevice, mDeviceContext);
	//mTestTriangle2 = new TestTriangle(mDevice, mDeviceContext);

	mPhysics = new PhysicsController2(mDevice, mDeviceContext);

	Restart();

	mDown = false;

	PreviousMousePos = Point();
	CurrentMousePos = Point();
	PreviousMousePos = CurrentMousePos = func();
	SetCursorPos(CurrentMousePos.x, CurrentMousePos.y);

	AglVector3 e1;
	AglVector3 e2;
	AglVector3 e3;
	float** mat = new float*[3];
	mat[0] = new float[3];
	mat[1] = new float[3];
	mat[2] = new float[3];

	mat[0][0] = 3;
	mat[0][1] = 2;
	mat[0][2] = 4;
	mat[1][0] = 2;
	mat[1][1] = 0;
	mat[1][2] = 2;
	mat[2][0] = 4;
	mat[2][1] = 2;
	mat[2][2] = 3;

	mat[0][0] = 7;
	mat[0][1] = -2;
	mat[0][2] = 0;
	mat[1][0] = -2;
	mat[1][1] = 6;
	mat[1][2] = -2;
	mat[2][0] = 0;
	mat[2][1] = -2;
	mat[2][2] = 5;

	FindSymmetricEigenVectors(mat, e1, e2, e3);

	ShowCursor(false);
	return true;
}
bool Game::Cleanup()
{
	delete mPhysics;
	delete mCameraController;
	Camera::Destroy();
	ShaderManager::Destroy();

	DX11Application::Cleanup();
	return true;
}
bool Game::Update(float pElapsedTime)
{
	Time += pElapsedTime;
	DX11Application::Update(pElapsedTime);
	if (!Avatar)
		mCameraController->Update(pElapsedTime);
	if (mPhysics)
		mPhysics->Update(pElapsedTime);

	if (!Avatar && mPhysics)
	{
		if(GetAsyncKeyState(VK_SPACE))
		{
			if (!mDown)
				mPhysics->AddBox(Camera::GetInstance()->GetPosition(), AglVector3(1, 1, 1), 1, Camera::GetInstance()->LocalZAxis()*20, AglVector3(0.0f, 0.0f, 0), false);
			mDown = true;
		}
		else if(GetAsyncKeyState('R') & 0x8000)
		{
			if (!mDown)
			{
				Restart();
				DX11Application::t1 = 0;
			}
			mDown = true;
		}
		else
			mDown = false;
	}

	if (Avatar && mPhysics)
	{
		CompoundBody* av = (CompoundBody*)mPhysics->GetBody(Avatar);
		AglMatrix m = av->GetWorld();
		AglMatrix world = AglMatrix(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9],
								m[10], m[11], m[12], m[13], m[14], m[15]);

		//Mouse
		GetCursorPos(&CurrentMousePos);
		int dy = CurrentMousePos.y - PreviousMousePos.y;
		int dx = CurrentMousePos.x - PreviousMousePos.x;
		PreviousMousePos = func();
		SetCursorPos(PreviousMousePos.x, PreviousMousePos.y);
		if (dy != 0)
		{
			av->AddAngularImpulse(-world.GetRight() * (float)dy * pElapsedTime*4);
		}
		if (dx != 0)
		{
			av->AddAngularImpulse(world.GetUp() * (float)dx * pElapsedTime*4);
		}


		if(GetAsyncKeyState('T') & 0x8000)
		{
			if (!mDown)
			{
				//this->mPhysics->DetachBodyFromCompound(Avatar, toDetach);
			}
			mDown = true;
		}
		else
			mDown = false;


		if(GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			av->AddImpulse(world.GetForward()*50.0f * av->GetMass() * pElapsedTime);
		}
		else if(GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		{
			av->AddImpulse(-world.GetForward()*50.0f * av->GetMass() * pElapsedTime);
		}

		if(GetAsyncKeyState('W') & 0x8000)
		{
			av->AddAngularImpulse(world.GetRight()*pElapsedTime*4);
			//Avatar->AddImpulse(Avatar->GetWorld().GetForward()*0.05f);
		}
		else if(GetAsyncKeyState('S') & 0x8000)
		{
			av->AddAngularImpulse(-world.GetRight()*pElapsedTime*4);
			//Avatar->AddImpulse(-Avatar->GetWorld().GetForward()*0.05f);
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			av->AddAngularImpulse(-world.GetUp()*pElapsedTime*4);
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			av->AddAngularImpulse(world.GetUp()*pElapsedTime*4);
		}
		if (GetAsyncKeyState('Q') & 0x8000)
		{
			av->AddAngularImpulse(world.GetForward()*pElapsedTime*4);
		}
		else if (GetAsyncKeyState('E') & 0x8000)
		{
			av->AddAngularImpulse(-world.GetForward()*pElapsedTime*4);
		}

		float distance = 10;
		AglVector3 newPos = world.GetTranslation() + world.GetBackward() * distance;
		//AglVector3 diff = newPos - Camera::GetInstance()->GetPosition();
		//Camera::GetInstance()->Init(Camera::GetInstance()->GetPosition() + diff * pElapsedTime * 20.0f, world.GetTranslation(), world.GetUp(), mScreenWidth, mScreenHeight);
		Camera::GetInstance()->Init(newPos, world.GetTranslation(), world.GetUp(), mScreenWidth, mScreenHeight);


		//Cast rays from sphere towards the camera
		AglVector3 start = world.GetTranslation();
		AglVector3 target = Camera::GetInstance()->GetPosition();
		AglVector3 dir = target-start;
		//Normalize(dir);
		AglVector3 dirnorm = dir;
		Normalize(dirnorm);

		AglBoundingSphere bs;
		bs.position = start + dirnorm * 5.0f;
		bs.radius = 5;

		float step = 1.0f / 4.0f;
		float offstart = -0.5f;
		vector<PhyRay> raylist;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				AglVector3 offset = world.GetRight() * (offstart+i*step) + world.GetUp() * (offstart+j*step); 
				PhyRay r;
				r.o = AglVector3(start + offset);
				r.d = dir;
				raylist.push_back(r);
			}
		}
		/*float minT = mPhysics->RaysVsObjects(raylist, Avatar, bs);
		if (minT < 1)
		{
			AglVector3 desired = start + dir * minT;
			Camera::GetInstance()->MoveTo(desired);
		}*/
	}



	return true;
}
bool Game::Draw(float pElapsedTime)
{
	DX11Application::Draw(pElapsedTime);
    //mDeviceContext->ClearRenderTargetView(mBackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	mDeviceContext->ClearRenderTargetView(mBackBuffer, color);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	//mDeviceContext->RSSetState(mRasterState);
	if (mPhysics)
		mPhysics->DrawDebug();

	testMesh->Draw(AglMatrix::createTranslationMatrix(AglVector3(20, 0, -40)));

    mSwapChain->Present(0,0);
	return true;
}

void Game::Restart()
{
	Avatar = NULL;
	DX11Application::hitbug = false;
	mPhysics->Clear();

	int val = 1;

	//Normal restart
	if (val == 0)
	{
		mPhysics->AddBox(AglVector3(10, 0, 0), AglVector3(2, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(-10, 0, 0), AglVector3(2, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 10, 0), AglVector3(20, 2, 20), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, -10, 0), AglVector3(20, 2, 20), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 0, 10), AglVector3(18, 18, 2), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);

		mPhysics->AddConvexHull(AglVector3(0, 4, 0), 1, 1, AglVector3(0.0f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(4, 4, 0), 1, 1, AglVector3(-0.5f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(-4, 4, 0), 1, 1, AglVector3(0.0f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(0, 4, 4), 1, 1, AglVector3(0.0f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(4, 4, 4), 1, 1, AglVector3(-0.5f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(-4, 4, 4), 1, 1, AglVector3(0.0f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(0, 4, -4), 1, 1, AglVector3(0.0f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(4, 4, -4), 1, 1, AglVector3(-0.5f, 0, 0), AglVector3(0, 0, 0));
		mPhysics->AddConvexHull(AglVector3(-4, 4, -4), 1, 1, AglVector3(0.0f, 0, 0), AglVector3(0, 0, 0));
	}
	else if (val == 1)
	{
		//Test camera etc
		mPhysics->AddBox(AglVector3(0, 0, 0), AglVector3(18, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 20, 0), AglVector3(18, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, -20, 0), AglVector3(18, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(-20, 0, 0), AglVector3(18, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(20, 0, 0), AglVector3(18, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 0, 20), AglVector3(18, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);


		mPhysics->AddBox(AglVector3(0, -100, 0), AglVector3(200, 1, 200), 200, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 100, 0), AglVector3(200, 1, 200), 200, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 0, -100), AglVector3(200, 200, 1), 200, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 0, 100), AglVector3(200, 200, 1), 200, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(100, 0,  0), AglVector3(1, 200, 200), 200, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(-100, 0, 0), AglVector3(1, 200, 200), 200, AglVector3(0,0,0), AglVector3(0, 0, 0), true);

		//Some movable spheres
		mPhysics->AddSphere(AglVector3(5, 0, -25), 1);
		mPhysics->AddSphere(AglVector3(-5, 0, -25), 1);

		//Some movable boxes
		mPhysics->AddBox(AglVector3(10, 0, -25), AglVector3(4, 4, 4), 1, AglVector3(0,0,0), AglVector3(0, 0, 0), false);

		//Some movable convex hulls
		mPhysics->AddConvexHull(AglVector3(-13, 0, -25), 7, 1, AglVector3(0,0,0), AglVector3(0, 0, 0), false);

		//Old
		//Avatar = mPhysics->AddSphere(AglVector3(0, 0, -25), 1.0f, true);

		//new
		Avatar = mPhysics->AddCompoundBody(AglVector3(0, 0, -40));
		CompoundBody* av = (CompoundBody*)mPhysics->GetBody(Avatar);
		float step = 1.0f / 9.0f * 2 * 3.14159f;
		for (unsigned int i = 0; i < 7; i++)
		{
			//toDetach =
			mPhysics->AddSphere(AglVector3(cos(step*i)*2.5f, sin(step*i)*2.5f, 0), 1.0f, false, av);
		}


		string file = openfilename("Agile Files (*.agl*)\0*.agl*\0");

		AglReader r((char*)file.c_str());

		AglScene* s = r.getScene();

		vector<AglMesh*> m = s->getMeshes();
		testMesh = new DebugMesh(mDevice, mDeviceContext, m[0]);
		mPhysics->AddMeshBody(AglVector3(20, 0, -40), m[0]->getHeader().minimumOBB, m[0]->getHeader().boundingSphere);
	}
	else if (val == 2)
	{
		mPhysics->AddBox(AglVector3(10, 0, 0), AglVector3(2, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(-10, 0, 0), AglVector3(2, 18, 18), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		//mPhysics->AddBox(AglVector3(0, 10, 0), AglVector3(20, 2, 20), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, -10, 0), AglVector3(20, 2, 20), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);
		mPhysics->AddBox(AglVector3(0, 0, 10), AglVector3(18, 18, 2), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);

		for (int i = 14; i >= 0; i--)
		{
			mPhysics->AddBox(AglVector3(2.0f, -8.49f + i * 1.010f, 0), AglVector3(1, 1, 1), 1, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		}
	}
	else
	{
		mPhysics->AddBox(AglVector3(0, -3, 0), AglVector3(20, 2, 20), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), true);

		//Base
		mPhysics->AddBox(AglVector3(-9.0f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(-7.5f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(-6.0f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(-4.5f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(-3.0f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(-1.5f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(0.0f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(9.0f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(7.5f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(6.0f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(4.5f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(3.0f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
		mPhysics->AddBox(AglVector3(1.5f, -1.4f, 0), AglVector3(1, 1, 1), 100, AglVector3(0,0,0), AglVector3(0, 0, 0), false);
	}

}