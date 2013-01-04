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
	AglVector3			spawnDirection;
	float				spawnSpeed;
	float				particleAge;
	float				spread; ///< 0 -> 1
	float				spawnFrequency;
	AglParticleFormat	particleFormat;
};

struct AglStandardParticle
{
	AglVector3	position;
	AglVector3	velocity;
	float		size;
	float		age;

	AglStandardParticle(AglVector3 p_position, AglVector3 p_velocity, float p_size);
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
	vector<AglStandardParticle> m_particles;
	float		  m_age;
	float		  m_timeSinceSpawn;
public:
	AglParticleSystem();
	AglParticleSystem(AglParticleSystemHeader p_header);
	virtual ~AglParticleSystem();

	AglParticleSystemHeader getHeader();
	vector<AglStandardParticle> getParticles();

	void setSpawnPoint(AglVector3 p_spawnPoint);
	void setParticleAge(float p_age);
	void setSpawnDirection(AglVector3 p_direction);
	void setSpawnSpeed(float p_speed);
	void setSpread(float p_spread);
	void setSpawnFrequency(float p_frequency);

	void update(float p_dt);
};

#endif // AGLPARTICLESYSTEM_H