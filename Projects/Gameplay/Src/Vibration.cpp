#include "Vibration.h"

ComponentRegister<Vibration> Vibration::s_reg("Vibration");

Vibration::Vibration()
	: Component( ComponentType::Vibration )
{
	enabled = false;
	leftPower=20.0f; // 20%
	rightPower=20.0f;
	maxRadius=10.0f;
	minRadius=1.0f;
	distanceAttenuated=false;
	linearDeclineEffectMultiplier=-1.0f;
	enableDeclineEffect=false;
}

Vibration::Vibration(float p_power, float p_minRadius, float p_maxRadius)
	: Component( ComponentType::Vibration )
{
	enabled=false;
	leftPower=p_power;
	rightPower=p_power;
	maxRadius=p_maxRadius;
	minRadius=p_minRadius;
	distanceAttenuated=true;
	linearDeclineEffectMultiplier=-1.0f;
	enableDeclineEffect=false;
}
void Vibration::init( vector<ComponentData> p_initData )
{
	char* cStr;
	int temp=0;
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		if( p_initData[i].dataName == "maxRadius" )
		{
			p_initData[i].getData<float>(&maxRadius);
			distanceAttenuated=true;
		}
		else if( p_initData[i].dataName == "minRadius" )
		{
			p_initData[i].getData<float>(&minRadius);
			distanceAttenuated=true;
		}
	}
}