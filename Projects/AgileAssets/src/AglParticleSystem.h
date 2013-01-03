#ifndef AGLPARTICLESYSTEM_H
#define AGLPARTICLESYSTEM_H

#include "AglVector3.h"
#include "Agl.h"
#include <vector>

using namespace std;

#define AGL_PARTICLE_FORMAT_STANDARD 0

struct AglParticleSystemHeader
{
	AglVector3			spawnPoint;
	float				particleAge;
	AglParticleFormat	particleFormat;
};

struct AglStandardParticle
{
	AglVector3	position;
	AglVector3	velocity;
	float		size;
	float		age;
};


// =======================================================================================
//                                      AglParticleSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Describes a particle system
///        
/// # AglParticleSystem
/// Created on: 3-1-2013 
///---------------------------------------------------------------------------------------
class AglParticleSystem
{
private:
	AglParticleSystemHeader m_header;
	vector<void*> m_particles;
public:
	AglParticleSystem();
	AglParticleSystem(AglParticleSystemHeader p_header);
	virtual ~AglParticleSystem();

	AglParticleSystemHeader getHeader();
	vector<void*> getParticles();

	void setSpawnPoint(AglVector3 p_spawnPoint);

	void update(float p_dt);
};

#endif // AGLPARTICLESYSTEM_H