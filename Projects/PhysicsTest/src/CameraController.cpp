#include "CameraController.h"

CameraController::CameraController(HWND pWindowHandle)
{
	mWindowHandle = pWindowHandle;
	mPreviousMousePos = mCurrentMousePos = GetWindowCenter();
	SetCursorPos(mPreviousMousePos.x, mPreviousMousePos.y);
}
void CameraController::Update(float pElapsedTime)
{
	float dt = pElapsedTime;
	GetCursorPos(&mCurrentMousePos);
	int dy = mCurrentMousePos.y - mPreviousMousePos.y;
	int dx = mCurrentMousePos.x - mPreviousMousePos.x;
	if (dy != 0)
	{
		Camera::GetInstance()->Tilt(-dy * 0.0087266f );
		float AngleAfter = AglVector3::dotProduct(Camera::GetInstance()->LocalYAxis(), AglVector3(0,1,0));
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
	SetCursorPos(mPreviousMousePos.x, mPreviousMousePos.y);
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