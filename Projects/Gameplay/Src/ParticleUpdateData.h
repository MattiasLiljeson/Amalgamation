#pragma once

#include <AglVector3.h>
#include <Component.h>



class ParticleUpdateData: public Component
{
public:
	ParticleUpdateData()
		: Component( ComponentType::ParticleUpdateData )
	{
<<<<<<< HEAD
		m_componentTypeId = ComponentType::ParticleUpdateData;
=======
>>>>>>> 5cd560c1d461cb00d2a0f9e606ce33647f38e483
	}

public:
	AglVector3 spawnPoint;
	AglVector3 direction;
	float	   speed;
	float	   spawnFrequency;
};