#include "MinigunModule.h"

ComponentRegister<MinigunModule> MinigunModule::s_reg("MinigunModule");

MinigunModule::MinigunModule()
	: Component( ComponentType::MinigunModule )	
{
	laserSightEntity=-1;
	animationPlaying = false;
	rayIndex = -1;
	timeSinceSound = 0;
	minigunState = IDLE;
}

MinigunModule::MinigunModule( AglVector3 p_muzzleOffset, AglVector3 p_fireDirection )
	: Component( ComponentType::MinigunModule )
{
	m_componentTypeId = ComponentType::MinigunModule;

	muzzleOffset = p_muzzleOffset;
	fireDirection = p_fireDirection;
	laserSightEntity = -1;
	range = 300;
	particleSystemEntity = -1;
	animationPlaying = false;
	rayIndex = -1;
	timeSinceSound = 0;
	minigunState = IDLE;
}
MinigunModule::~MinigunModule()
{

}

void MinigunModule::init( vector<ComponentData> p_initData )
{
	muzzleOffset = AglVector3(0, 0, 0);
	fireDirection = AglVector3(0, 0, 1);
	laserSightEntity = -1;
	range = 300;
	particleSystemEntity = -1;
	animationPlaying = false;
	rayIndex = -1;
	timeSinceSound = 0;

	for(unsigned int i=0; i<p_initData.size(); i++)
	{
	}
}