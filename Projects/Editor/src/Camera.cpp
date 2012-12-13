#include "Camera.h"

Camera* Camera::mInstance = NULL;

Camera::Camera()
{
	mPosition = AglVector3(0,0,0);
	mTarget =  AglVector3(0,0,0);
	mUpDirection = AglVector3(0,0,0);

	mViewNeedsUpdate = true;
}
void Camera::Init(AglVector3 Position, AglVector3 Target, AglVector3 WorldUpVector, int Width, int Height)
{
	mPosition = Position;
	mTarget = Target;
	mUpDirection = WorldUpVector;

	float r = (float)Width / (float)Height;
	mViewAngle = PI * 0.25f;
	mProj = AglMatrix(1.0f/(float)(r*tan(mViewAngle/2)), 0.0f, 0.0f, 0.0f, 
						0.0f, 1.0f/(float)tan(mViewAngle/2), 0.0f, 0.0f, 
							0.0f, 0.0f, 1000.0f/(1000.0f-0.1f), 1.0f, 
								0.0f, 0.0f, (-0.1f*1000.0f)/(1000.0f - 0.1f), 0.0f);
	mViewNeedsUpdate = true;
}
Camera* Camera::GetInstance()
{
	if (!mInstance)
		mInstance = new Camera();
	return mInstance;
}
void Camera::Move(AglVector3 Distance)
{
	mPosition += Distance;
	mTarget += Distance;
	mViewNeedsUpdate = true;
}
AglMatrix Camera::GetCameraMatrix()
{
	return GetViewMatrix()*mProj;
}
AglMatrix Camera::GetProjectionMatrix()
{
	return mProj;
}
AglMatrix Camera::GetViewMatrix()
{
	if (mViewNeedsUpdate)
	{
		//Local Camera z-axis
		AglVector3 w = (mTarget-mPosition) / Length(mTarget-mPosition);

		//Local Camera x-axis
		AglVector3 u = CrossProduct(mUpDirection, w);
		u /= Length(u);

		//Local Camera y-axis
		AglVector3 v = CrossProduct(w, u);
		v /= Length(v);
		mView = AglMatrix(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, 
			u.z, v.z, w.z, 0, -DotProduct(u, mPosition), 
				-DotProduct(v, mPosition), -DotProduct(w, mPosition), 1);

		mViewNeedsUpdate = false;
	}
	return mView;
}
AglVector3 Camera::LocalXAxis()
{
	AglVector3 u = CrossProduct(mUpDirection, LocalZAxis());
	return u /= Length(u);
}
AglVector3 Camera::LocalYAxis()
{
	AglVector3 v = CrossProduct(LocalZAxis(), LocalXAxis());
	return v /= Length(v);
}
AglVector3 Camera::LocalZAxis()
{
	return (mTarget-mPosition) / Length(mTarget-mPosition);
}
void Camera::MoveLeft(float Distance)
{
	AglVector3 CameraDirection = mTarget - mPosition;
	AglVector3 MoveDirection = CrossProduct(CameraDirection, mUpDirection);
	Normalize(MoveDirection);
	Move(MoveDirection*Distance);
	mViewNeedsUpdate = true;
}
void Camera::MoveRight(float Distance)
{
	//Inverted move left
	MoveLeft(-Distance);
}
void Camera::MoveForwards(float Distance)
{
	AglVector3 MoveDirection = mTarget - mPosition;
	Normalize(MoveDirection);
	Move(MoveDirection*Distance);
	mViewNeedsUpdate = true;
}
void Camera::MoveForwardsMaintainTarget(float Distance)
{
	AglVector3 MoveDirection = mTarget - mPosition;
	Normalize(MoveDirection);
	mPosition += MoveDirection*Distance;
	mViewNeedsUpdate = true;
}
void Camera::MoveBackwards(float Distance)
{
	//Inverted move forwards
	MoveForwards(-Distance);
}
void Camera::MoveUp(float Distance)
{
	Move(mUpDirection*Distance);
	mViewNeedsUpdate = true;
}
void Camera::MoveDown(float Distance)
{
	MoveUp(-Distance);
}
void Camera::Panorate(float Angle)
{
	AglVector3 CameraDirection = mTarget - mPosition;
	AglVector3 rotationAxis = mUpDirection;
	Normalize(rotationAxis);
	Rotate(CameraDirection, Angle, rotationAxis);
	mTarget = mPosition + CameraDirection;
	mViewNeedsUpdate = true;
}
void Camera::Tilt(float Angle)
{
	AglVector3 CameraDirection = mTarget - mPosition;
	AglVector3 axis = CrossProduct(CameraDirection, mUpDirection);
	Normalize(axis);
	Rotate(mUpDirection, Angle, axis);
	Rotate(CameraDirection, Angle, axis);
	mTarget = mPosition + CameraDirection;
	mViewNeedsUpdate = true;
}
void Camera::Roll(float Angle)
{
	AglVector3 axis = mTarget - mPosition;
	Rotate(mUpDirection, Angle, axis);
	mViewNeedsUpdate = true;
}
void Camera::SetY(float y)
{ 
	float diff = y - mPosition.y;
	mPosition.y += diff; 
	mTarget.y += diff;
	mViewNeedsUpdate = true;
}
//Rotates around an arbitrary axis using quaternions
void Camera::ArbitraryRotatation(float Angle, AglVector3 Axis)
{
	AglVector3 CameraDirection = mTarget-mPosition;
	Rotate(CameraDirection, Angle, Axis);
	Rotate(mUpDirection, Angle, Axis);
	mTarget = mPosition + CameraDirection;
	mViewNeedsUpdate = true;
}
void Camera::Release()
{
	if (mInstance)
		delete mInstance;
	mInstance = NULL;
}
//Essentially sets the projection matrix
void Camera::SetLens(float Angle, int Width, int Height, int n, int f)
{
		mViewAngle = Angle;
		float r = (float)Width / (float)Height;
		mProj = AglMatrix(1/(r*tan(Angle/2)), 0.0f, 0.0f, 0.0f, 
						0.0f, 1.0f/tan(Angle/2), 0.0f, 0.0f, 
							0.0f, 0.0f, f/(float)(f-n), 1.0f, 
								0.0f, 0.0f, (-n*f)/(float)(f - n), 0.0f);
}
float Camera::GetViewAngle()
{
	return (float)mViewAngle;
}
void Camera::MoveTo(AglVector3 pos)
{
	AglVector3 diff = pos - mPosition;
	mPosition += diff;
	mTarget += diff;
	mViewNeedsUpdate = true;
}
void Camera::MoveToPreserveTarget(AglVector3 pPosition)
{
	mPosition = pPosition;
	mViewNeedsUpdate = true;
}