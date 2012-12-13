#ifndef CAMERA_H
#define CAMERA_H

#include "Utility.h"

class Camera
{
public:
	static Camera* GetInstance();
	static void	   Release();
	void Init(AglVector3 Position, AglVector3 Target, AglVector3 WorldUpVector, int Width, int Height);
	AglMatrix GetCameraMatrix();
	AglMatrix GetProjectionMatrix();
	AglMatrix GetViewMatrix();

	//Returns the local axises of the camera.
	AglVector3 LocalXAxis();
	AglVector3 LocalYAxis();
	AglVector3 LocalZAxis();

	//Move Function relative to world space
	void Move(AglVector3 Distance);

	//Move Functions relative to camera space
	void MoveLeft(float Distance);
	void MoveRight(float Distance);
	void MoveForwards(float Distance);
	void MoveForwardsMaintainTarget(float Distance);
	void MoveBackwards(float Distance);
	void MoveUp(float Distance);
	void MoveDown(float Distance);

	//Rotating functions
	void Panorate(float Angle);
	void Tilt(float Angle);
	void Roll(float Angle);
	void ArbitraryRotatation(float Angle, AglVector3 Axis);

	AglVector3 Position() const{ return mPosition; }
	void	MoveTo(AglVector3 pos);
	void SetY(float y);
	void SetLens(float Angle, int Width, int Height, int n, int f);
	float GetViewAngle();
	AglVector3 GetPosition(){ return mPosition; }
	AglVector3 GetTarget(){ return mTarget;}

	void MoveToPreserveTarget(AglVector3 pPosition);

private:
	Camera();
private:
	AglVector3 mPosition;
	AglVector3 mTarget;
	AglVector3 mUpDirection;
	AglMatrix mView;
	AglMatrix mProj;
	double mViewAngle;
	static Camera* mInstance;

	bool mViewNeedsUpdate;

};

#endif