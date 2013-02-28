#ifndef AGLPARTICLESYSTEM_H
#define AGLPARTICLESYSTEM_H

#include "AglVector3.h"
#include "Agl.h"
#include <vector>
#include "AglParticleSystemHeader.h"
#include "AglStandardParticle.h"

using namespace std;

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

	AglParticleSystemHeader* getHeaderPtr();
	const AglParticleSystemHeader& getHeader() const;
	vector<AglStandardParticle>* getParticlesPtr();
	const vector<AglStandardParticle>& getParticles();
	const float getAge() const;

	void setSpawnPoint(AglVector3 p_spawnPoint);
	void setParticleAge(float p_age);
	const AglVector3& getSpawnDirection();
	const AglVector3& getCurrSpawnDirection();
	void setSpawnDirection(AglVector3 p_direction);
	void setCurrSpawnDirection(AglVector3 p_direction);
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
	void setSpawnAngularVelocity(float p_angularVelocity);
	void setColor(AglVector4 p_color);
	void setFadeOutStart(float p_fadeOutStart);
	void setFadeInStop(float p_fadeInStop);
	void setTextureNameIndex(int p_textureNameIndex);
	void setAlignmentType(AglParticleSystemHeader::AglAlignmentType p_alignment);
	void setRasterizerMode(AglParticleSystemHeader::AglRasterizerMode p_mode);

	AglParticleSystemHeader::AglSpace getSpawnSpace();
	void setSpawnSpace( AglParticleSystemHeader::AglSpace p_space );
	AglParticleSystemHeader::AglSpace getParticleSpace();
	void setParticleSpace( AglParticleSystemHeader::AglSpace p_space );
	void setModes( AglParticleSystemHeader::AglBlendMode p_blendMode,
		AglParticleSystemHeader::AglRasterizerMode p_rasterizerMode );

	float getCameraDistance(AglVector3 p_cameraPos, AglVector3 p_cameraDir);

	void restart();

	void update(float p_dt, AglVector3 p_cameraPosition);
};

#endif // AGLPARTICLESYSTEM_H