#pragma once

#include <AglVector3.h>
#include <Component.h>
#include <utility>
#include <vector>
#include <AglParticleSystemHeader.h>
#include <ParticleSystemInstruction.h>

struct ParticleSystemData
{
	ParticleSystemData()
	{
		name = "NOT INITIALIZED!";
	}

	ParticleSystemData( const AglParticleSystemHeader& p_header, const string& p_name )
	{
		originalSettings = p_header;
		updateData = p_header;
		name = p_name;
	}

	ParticleSystemData( const ParticleSystemInstruction& p_instruction, const string& p_name )
	{
		originalSettings = AglParticleSystemHeader( p_instruction.particleSystem.getHeader() );
		updateData = originalSettings;
		name = p_name;
	}

	AglParticleSystemHeader originalSettings;
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
	/// If the ps already exists, it's current ID will be returned.
	int addParticleSystem( const ParticleSystemData& p_particleSystem, int p_idx = -1 )
	{
		int idx = getParticleSystemIdxFromName( p_particleSystem.name );
		if( idx == -1) // If the ps isn't found
		{
			if( p_idx == -1) { // If a preferred index hasn't been specified
				// Place the new ps at the back
				idx = particleSystems.size();
				particleSystems.push_back( p_particleSystem );
			} else { // If a preferred idx has been supplied
				idx = p_idx;
				if( idx >= particleSystems.size() ) { // If idx out of current range
					// Then expand and place the new ps at the supplied idx
					particleSystems.resize( idx+1 );
					particleSystems[idx] = p_particleSystem;
				} else { // Else, idx is in range 
					// Then move the current ps residing at the supplied idx to the back 
					// before placing the new ps at the given idx,
					particleSystems.push_back( particleSystems[idx] );
					particleSystems[idx] = p_particleSystem;
				}
			}
		} else { // A ps with the same name already exists.
			// Replace the current ps with the new one
			particleSystems[idx] = p_particleSystem;
		}
		return idx;
	}

	/// Get particle system by its idx. If not found (idx out of range), NULL will be returned.
	ParticleSystemData* getParticleSystemFromIdx( int p_idx )
	{
		if( 0 <= p_idx && p_idx < particleSystems.size() ) {
			return &particleSystems[p_idx];
		}
		return NULL;
	}

	/// Fetch ps by name. SLOW! O(N)
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

	/// Fetch ps idx by name. SLOW! O(N)
	int getParticleSystemIdxFromName( const string& p_name )
	{
		for( unsigned int i=0; i<particleSystems.size(); i++ ) {
			if( particleSystems[i].name == p_name ) {
				return i;
			}
		}
		return -1;
	}

	int getParticleSystemCnt()
	{
		return particleSystems.size();
	}

	vector< ParticleSystemData > particleSystems;
};