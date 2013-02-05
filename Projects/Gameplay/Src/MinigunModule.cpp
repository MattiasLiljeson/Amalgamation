#include "MinigunModule.h"

MinigunModule::MinigunModule()
	: Component( ComponentType::MinigunModule )	
{
	coolDown=0.0f;
	laserSightEntity=-1;
}

MinigunModule::MinigunModule( AglVector3 p_muzzleOffset, AglVector3 p_fireDirection )
	: Component( ComponentType::MinigunModule )
{
	m_componentTypeId = ComponentType::MinigunModule;

	muzzleOffset = p_muzzleOffset;
	fireDirection = p_fireDirection;
	laserSightEntity = -1;
	coolDown = 0;
	currentRay = 0;
	range = 300;
	particleSystemEntity = -1;
}
MinigunModule::~MinigunModule()
{

}