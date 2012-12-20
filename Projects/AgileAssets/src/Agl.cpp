#include "Agl.h"

AglCoordinateSystem::AglCoordinateSystem()
{
	upVector = AglVector3(0, 1, 0);
	forwardVector = AglVector3(0, 0, 1);
	handedness = LEFT;
}

AglCoordinateSystem AglCoordinateSystem::DX()
{
	AglCoordinateSystem s;
	s.upVector = AglVector3(0, 1, 0);
	s.forwardVector = AglVector3(0, 0, 1);
	s.handedness = LEFT;
	return s;
}
AglCoordinateSystem AglCoordinateSystem::GL()
{
	AglCoordinateSystem s;
	s.upVector = AglVector3(0, 1, 0);
	s.forwardVector = AglVector3(0, 0, -1);
	s.handedness = RIGHT;
	return s;
}
AglCoordinateSystem AglCoordinateSystem::BLENDER()
{
	AglCoordinateSystem s;
	s.upVector = AglVector3(0, 0, 1);
	s.forwardVector = AglVector3(0, 1, 0);
	s.handedness = RIGHT;
	return s;
}