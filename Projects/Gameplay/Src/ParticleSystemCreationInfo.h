#pragma once
#include <AglParticleSystemHeader.h>
#include <string>

struct ParticleSystemCreationInfo
{
	// Network unique ID
	int entityNetId;
	
	
	// Internal index in the particle system component in the entity. If there are local
	// particle systems this will not work.
	int particleSysIdx;
	string textureFileName;
	AglParticleSystemHeader particleSysHeader;
};