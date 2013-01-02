#ifndef GAME_H
#define GAME_H

#include "DX11Application.h"
#include "PhysicsController2.h"
#include "Utility.h"
#include "DebugMesh.h"

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
	int Avatar;
	int toDetach;
	int mesh;

	Point PreviousMousePos;
	Point CurrentMousePos;

	DebugMesh* testMesh;
	DebugMesh* toDraw;

	AglMatrix coord;

	AglOBB bound;

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