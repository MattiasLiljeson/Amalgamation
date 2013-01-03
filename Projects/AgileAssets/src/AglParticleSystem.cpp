#include "AglParticleSystem.h"

AglParticleSystem::AglParticleSystem()
{
	m_header.particleAge = 0;
	m_header.spawnPoint = AglVector3(0, 0, 0);
	m_header.particleFormat = AGL_PARTICLE_FORMAT_STANDARD;
}
AglParticleSystem::AglParticleSystem(AglParticleSystemHeader p_header)
{
	m_header = p_header;
}
AglParticleSystem::~AglParticleSystem()
{

}

AglParticleSystemHeader AglParticleSystem::getHeader()
{
	return m_header;
}
vector<void*> AglParticleSystem::getParticles()
{
	return m_particles;
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
	}
	else
	{
		//Do nothing
	}
}