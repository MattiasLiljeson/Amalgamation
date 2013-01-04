#include "CameraController.h"
#include "Globals.h"
#include "Scene.h"

CameraController::CameraController(HWND pWindowHandle)
{
	mWindowHandle = pWindowHandle;
	mPreviousMousePos = mCurrentMousePos = GetWindowCenter();
	SetCursorPos(mPreviousMousePos.x, mPreviousMousePos.y);
	mActive = false;
	mPivot = AglVector3(0, 0, 0);
}
void CameraController::Update(float pElapsedTime)
{
	float dt = pElapsedTime;
	float length = (Camera::GetInstance()->GetPosition()-mPivot).length();

	float factor = min(length / 2.25f, 1.0f);

	Camera::GetInstance()->MoveForwards((MOUSEWHEELDELTA)*dt*0.3f);
	if(GetAsyncKeyState(VK_MBUTTON) & 0x8000)
	{
		GetCursorPos(&mCurrentMousePos);
		int dy = mCurrentMousePos.y - mPreviousMousePos.y;
		int dx = mCurrentMousePos.x - mPreviousMousePos.x;

		if (mActive)
		{
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				Camera::GetInstance()->MoveRight(dx*dt);
				Camera::GetInstance()->MoveUp(-dy*dt);
			}
			else
			{
				AglVector3 target = Camera::GetInstance()->GetTarget();
				AglVector3 dir = Camera::GetInstance()->GetPosition() - target;
				dir.normalize();
				target = Camera::GetInstance()->GetPosition() - dir * length;

				if (dx != 0)
				{
					AglQuaternion rot = AglQuaternion::constructFromAxisAndAngle(Camera::GetInstance()->LocalYAxis(), dt*dx);
					rot.transformVector(dir);
				}
				if (dy != 0)
				{
					AglQuaternion rot = AglQuaternion::constructFromAxisAndAngle(Camera::GetInstance()->LocalXAxis(), dt*dy);
					rot.transformVector(dir);
				}
				dir.normalize();
				Camera::GetInstance()->Init(target + dir*length, target, Camera::GetInstance()->LocalYAxis(), 800, 600);
			}
			mPivot = Camera::GetInstance()->GetTarget();
		}

		mPreviousMousePos = GetWindowCenter();
		SetCursorPos(mPreviousMousePos.x, mPreviousMousePos.y);
		mActive = true;
	}
	else if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		GetCursorPos(&mCurrentMousePos);
		int dy = mCurrentMousePos.y - mPreviousMousePos.y;
		int dx = mCurrentMousePos.x - mPreviousMousePos.x;

		if (mActive)
		{
			Camera::GetInstance()->Roll(dx * dt);
		}

		mPreviousMousePos = GetWindowCenter();
		SetCursorPos(mPreviousMousePos.x, mPreviousMousePos.y);
		mActive = true;
	}
	else
	{
		mActive = false;
	}

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		AglVector3 c = Scene::GetInstance()->GetCenter();
		Camera::GetInstance()->Init(c - AglVector3(0, 0.0f, 2.25f), c, AglVector3(0, 1, 0), 800, 600);
		mPivot = c;
	}

	//ShowCursor(!mActive);
}
Point CameraController::GetWindowCenter()
{
	RECT boundingbox;
	GetWindowRect(mWindowHandle, &boundingbox);
	Point center;
	center.x = (boundingbox.left+boundingbox.right)/2;
	center.y = (boundingbox.top+boundingbox.bottom)/2;
	return center;

}


/*float dt = pElapsedTime;
GetCursorPos(&mCurrentMousePos);
int dy = mCurrentMousePos.y - mPreviousMousePos.y;
int dx = mCurrentMousePos.x - mPreviousMousePos.x;
if (dy != 0)
{
	Camera::GetInstance()->Tilt(-dy * 0.0087266f );
	float AngleAfter = DotProduct(Camera::GetInstance()->LocalYAxis(), AglVector3(0,1,0));
	if (AngleAfter < 0.0f)
	{
		Camera::GetInstance()->Tilt(dy * 0.0087266f );
	}

}
if (dx != 0)
	Camera::GetInstance()->ArbitraryRotatation(dx * 0.0087266f, AglVector3(0,1,0));

AglVector3 prevCameraPos = Camera::GetInstance()->Position();
if(GetAsyncKeyState('A') & 0x8000)	Camera::GetInstance()->MoveLeft(6*dt);
if(GetAsyncKeyState('D') & 0x8000)	Camera::GetInstance()->MoveRight(6*dt);
if(GetAsyncKeyState('W') & 0x8000)	Camera::GetInstance()->MoveForwards(6*dt);
if(GetAsyncKeyState('S') & 0x8000)	Camera::GetInstance()->MoveBackwards(6*dt);

mPreviousMousePos = GetWindowCenter();
SetCursorPos(mPreviousMousePos.x, mPreviousMousePos.y);*/