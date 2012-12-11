#ifndef GAME_H
#define GAME_H

#include "DX11Application.h"
#include "PhysicsController2.h"
#include "Utility.h"

class Game: public DX11Application
{
private:
	CameraController*		mCameraController;

	PhysicsController2*		mPhysics;

	float					mTestFloat;
	//Temp
	//ID3D11RasterizerState* mRasterState;
	bool mDown;

	//HACK: TEMP TEMP TEMP FUCKING TEMP!!
	CompoundBody* Avatar;
	RigidBody* toDetach;

	Point PreviousMousePos;
	Point CurrentMousePos;

public:
	Game(HINSTANCE pInstanceHandle);
	bool Initialize();
	bool Cleanup();
	bool Update(float pElapsedTime);
	bool Draw(float pElapsedTime);

	void Restart();

	static float Time;


	Point func()
	{
		RECT boundingbox;
		GetWindowRect(mWindowHandle, &boundingbox);
		Point center;
		center.x = (boundingbox.left+boundingbox.right)/2;
		center.y = (boundingbox.top+boundingbox.bottom)/2;
		return center;
	}
};

#endif