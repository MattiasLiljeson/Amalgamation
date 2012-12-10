#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Camera.h"

class CameraController
{
private:
	HWND mWindowHandle;
	POINT mPreviousMousePos;
	POINT mCurrentMousePos;
private:
	POINT GetWindowCenter();
public:
	CameraController(HWND pWindowHandle);
	void Update(float pElapsedTime);
};

#endif