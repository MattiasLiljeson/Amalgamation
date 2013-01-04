#include "AglParticleSystem.h"

AglStandardParticle::AglStandardParticle(AglVector3 p_position, AglVector3 p_velocity, float p_size)
{
	position = p_position;
	velocity = p_velocity;
	size = p_size;
	age = 0;
}

AglParticleSystem::AglParticleSystem()
{
	m_header.particleAge = 0;
	m_header.spawnPoint = AglVector3(0, 0, 0);
	m_header.particleFormat = AGL_PARTICLE_FORMAT_STANDARD;
	m_age = 0;
	m_timeSinceSpawn = 0;
}
AglParticleSystem::AglParticleSystem(AglParticleSystemHeader p_header)
{
	m_header = p_header;
	m_age = 0;
	m_timeSinceSpawn = 0;
}
AglParticleSystem::~AglParticleSystem()
{
	if (m_header.particleFormat == AGL_PARTICLE_FORMAT_STANDARD)
	{
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			AglStandardParticle* p = (AglStandardParticle*)m_particles[i];
			delete p;
		}
	}
}

AglParticleSystemHeader AglParticleSystem::getHeader()
{
	return m_header;
}
vector<void*> AglParticleSystem::getParticles()
{
	return m_particles;
}
	
void AglParticleSystem::setSpawnPoint(AglVector3 p_spawnPoint)
{
	m_header.spawnPoint = p_spawnPoint;
}

void AglParticleSystem::update(float p_dt)
{
	if (m_header.particleFormat == AGL_PARTICLE_FORMAT_STANDARD)
	{
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			AglStandardParticle* p = (AglStandardParticle*)m_particles[i];
			
			if (p->age > m_header.particleAge) //Kill Particle
			{
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
			else
			{
				p->position += p->velocity * p_dt;
			}
		}

		m_age += p_dt;
		m_timeSinceSpawn += p_dt;
		if (m_timeSinceSpawn > 1.0f)
		{
			AglStandardParticle* p = new AglStandardParticle(m_header.spawnPoint, AglVector3(1, 0, 0), 0.02f);
			m_particles.push_back(p);
			m_timeSinceSpawn -= 1.0f;
		}

	}
	else
	{
		//Do nothing
	}
}