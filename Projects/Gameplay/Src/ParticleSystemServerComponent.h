#pragma once

#include <AglVector3.h>
#include <Component.h>
#include <utility>
#include <vector>
#include <AglParticleSystemHeader.h>

struct ParticleSystemUpdateData
{
	AglVector3 spawnPoint;
	AglVector3 direction;
	float	   speed;
	float	   spawnFrequency;
};

struct ParticleSystemData
{
	ParticleSystemData( const string& p_name )
	{
		name = p_name;
	}

	ParticleSystemData( const ParticleSystemUpdateData& p_updateData, const string& p_name )
	{
		updateData = p_updateData;
		name = p_name;
	}

	ParticleSystemUpdateData updateData;
	string name; // Used to fetch a PS by name, expensive!
};

class ParticleSystemServerComponent: public Component
{
public:
	ParticleSystemServerComponent()
	{
		m_componentTypeId = ComponentType::ParticleSystemServerComponent;
	}

	/// Add particle system to component. \return Index of the newly added particle system.
	int addParticleSystem( const ParticleSystemData& p_particleSystem)
	{
		int index = particleSystems.size();
		particleSystems.push_back( p_particleSystem );
		return index;
	}

	ParticleSystemData* getParticleSystemDataPtrFromName( const string& p_name )
	{
		ParticleSystemData* data = NULL;

		for( unsigned int i=0; i<particleSystems.size(); i++ ) {
			if( particleSystems[i].name == p_name ) {
				return &particleSystems[i];
			}
		}
	}

	vector< ParticleSystemData > particleSystems;
};