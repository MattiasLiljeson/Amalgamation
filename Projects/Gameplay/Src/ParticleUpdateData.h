#pragma once

#include <AglVector3.h>
#include <Component.h>



class ParticleUpdateData: public Component
{
public:
	ParticleUpdateData()
		: Component( ComponentType::ParticleUpdateData )
	{
	}

public:
	AglVector3 spawnPoint;
	AglVector3 direction;
	float	   speed;
	float	   spawnFrequency;
};