#include "MinigunModule.h"

MinigunModule::MinigunModule(AglVector3 p_muzzleOffset, AglVector3 p_fireDirection)
{
	muzzleOffset = p_muzzleOffset;
	fireDirection = p_fireDirection;
	laserSightEntity = -1;
	coolDown = 0;
	currentRay = 0;
	for (unsigned int i = 0; i < 10; i++)
	{
		rays[i].energy = 0.0f;
	}
}
MinigunModule::~MinigunModule()
{

}