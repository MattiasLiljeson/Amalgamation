#include "MinigunModule.h"

ComponentRegister<MinigunModule> MinigunModule::s_reg("MinigunModule");

MinigunModule::MinigunModule()
	: Component( ComponentType::MinigunModule )	
{
	coolDown=0.0f;
	laserSightEntity=-1;
	animationPlaying = false;
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
	animationPlaying = false;
}
MinigunModule::~MinigunModule()
{

}

void MinigunModule::init( vector<ComponentData> p_initData )
{
	muzzleOffset = AglVector3(0, 0, 0);
	fireDirection = AglVector3(0, 0, 1);
	laserSightEntity = -1;
	coolDown = 0;
	currentRay = 0;
	range = 300;
	particleSystemEntity = -1;
	animationPlaying = false;

	for(unsigned int i=0; i<p_initData.size(); i++)
	{
	}
}