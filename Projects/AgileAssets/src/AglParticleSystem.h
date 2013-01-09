#ifndef AGLPARTICLESYSTEM_H
#define AGLPARTICLESYSTEM_H

#include "AglVector3.h"
#include "Agl.h"
#include <vector>

using namespace std;

#define AGL_PARTICLE_FORMAT_STANDARD 0

struct AglParticleSystemHeader
{
	enum AglSpawnType
	{
		ONCE, CONTINUOUSLY
	};
	enum AglSpawnOffsetType
	{
		ONSPHERE, INSPHERE, ONRING, INRING
	};
	enum AglSpreadType
	{
		INPLANE, INSPACE
	};

	enum AglAlignmentType
	{
		SCREEN, WORLD, VELOCITY, OBSERVER
	};


	AglVector3			spawnPoint;
	AglSpawnType		spawnType;
	float				spawnOffset;
	AglSpawnOffsetType	spawnOffsetType;
	AglVector3			spawnDirection;
	float				spawnSpeed;
	float				particleAge;
	float				spread; ///< 0 -> 1
	AglSpreadType		spreadType;
	float				spawnFrequency;
	AglParticleFormat	particleFormat;
	float				maxOpacity; ///< 0 -> 1
	unsigned int		particlesPerSpawn;
	AglVector2			particleSize;
};

struct AglStandardParticle
{
	AglVector3	position;
	AglVector3	velocity;
	AglVector2	size;
	float		age;

	AglStandardParticle(AglVector3 p_position, AglVector3 p_velocity, AglVector2 p_size);
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

private:
	AglVector3 requestSpawnPoint();
	AglVector3 requestSpawnVelocity();
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
	void setSpawnType(AglParticleSystemHeader::AglSpawnType p_type);
	void setSpawnOffset(float p_offset);
	void setMaxOpacity(float p_maxOpacity);
	void setOffsetType(AglParticleSystemHeader::AglSpawnOffsetType p_type);
	void setParticlesPerSpawn(unsigned int p_particlesPerSpawn);
	void setSpreadType(AglParticleSystemHeader::AglSpreadType p_type);
	void setParticleSize(AglVector2 p_size);

	void restart();

	void update(float p_dt, AglVector3 p_cameraPosition);
};

#endif // AGLPARTICLESYSTEM_H