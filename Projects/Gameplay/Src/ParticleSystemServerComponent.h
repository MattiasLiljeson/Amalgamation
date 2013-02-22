#pragma once

#include <AglVector3.h>
#include <Component.h>
#include <utility>
#include <vector>
#include <AglParticleSystemHeader.h>
#include <ParticleSystemInstruction.h>

struct ParticleSystemData
{
	ParticleSystemData( const AglParticleSystemHeader& p_updateData, const string& p_name )
	{
		updateData = p_updateData;
		name = p_name;
	}

	ParticleSystemData( const ParticleSystemInstruction& p_instruction, const string& p_name )
	{
		updateData = AglParticleSystemHeader( p_instruction.particleSystem.getHeader() );
		name = p_name;
	}

	AglParticleSystemHeader updateData;
	string name; // Used to fetch a PS by name, expensive!
};

class ParticleSystemServerComponent: public Component
{
public:
	ParticleSystemServerComponent()
		: Component( ComponentType::ParticleSystemServerComponent )
	{
	}

	/// Add particle system to component. \return Index of the newly added particle system.
	int addParticleSystem( const ParticleSystemData& p_particleSystem)
	{
		int idx = getParticleSystemIdxFromName( p_particleSystem.name );
		if( idx == -1) {
			idx = particleSystems.size();
			particleSystems.push_back( p_particleSystem );
		} else {
			particleSystems[idx].updateData = p_particleSystem.updateData;
		}
		return idx;
	}

	ParticleSystemData* getParticleSystemDataPtrFromName( const string& p_name )
	{
		ParticleSystemData* data = NULL;
		int idx = getParticleSystemIdxFromName( p_name );
		if(idx != -1) {
			return &particleSystems[idx];
		} else {
			return NULL;
		}
	}

	int getParticleSystemIdxFromName( const string& p_name )
	{
		for( unsigned int i=0; i<particleSystems.size(); i++ ) {
			if( particleSystems[i].name == p_name ) {
				return i;
			}
		}
		return -1;
	}

	vector< ParticleSystemData > particleSystems;
};