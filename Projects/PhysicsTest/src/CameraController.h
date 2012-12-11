#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "Camera.h"

class CameraController
{
private:
	HWND mWindowHandle;
	Point mPreviousMousePos;
	Point mCurrentMousePos;
private:
	Point GetWindowCenter();
public:
	CameraController(HWND pWindowHandle);
	void Update(float pElapsedTime);
};

#endif