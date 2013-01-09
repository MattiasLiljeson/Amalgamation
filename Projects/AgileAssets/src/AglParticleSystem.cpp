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
	m_header.particleAge = 4.0f;
	m_header.spawnPoint = AglVector3(0, 0, 0);
	m_header.particleFormat = AGL_PARTICLE_FORMAT_STANDARD;
	m_header.spawnDirection = AglVector3::left();
	m_header.spawnSpeed = 0;
	m_header.spread = 0;
	m_header.spawnFrequency = 1.0f;
	m_header.spawnOffset = 0.0f;
	m_header.spawnOffsetType = AglParticleSystemHeader::ONSPHERE;
	m_header.spawnType = AglParticleSystemHeader::CONTINUOUSLY;
	m_header.maxOpacity = 1.0f;
	m_header.particlesPerSpawn = 1;
	m_header.spreadType = AglParticleSystemHeader::INSPACE;
	m_header.particleSize = 1.0f;
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
}

AglParticleSystemHeader AglParticleSystem::getHeader()
{
	return m_header;
}
vector<AglStandardParticle> AglParticleSystem::getParticles()
{
	return m_particles;
}
	
void AglParticleSystem::setSpawnPoint(AglVector3 p_spawnPoint)
{
	m_header.spawnPoint = p_spawnPoint;
}
void AglParticleSystem::setParticleAge(float p_age)
{
	m_header.particleAge = p_age;
}
void AglParticleSystem::setSpawnDirection(AglVector3 p_direction)
{
	p_direction.normalize();
	m_header.spawnDirection = p_direction;
}
void AglParticleSystem::setSpawnSpeed(float p_speed)
{
	if (p_speed >= 0)
		m_header.spawnSpeed = p_speed;
}
void AglParticleSystem::setSpread(float p_spread)
{
	if (p_spread >= 0 && p_spread <= 1.0f)
		m_header.spread = p_spread;
}
void AglParticleSystem::setSpawnFrequency(float p_frequency)
{
	if (p_frequency >= 0)
		m_header.spawnFrequency = p_frequency;
}
void AglParticleSystem::setMaxOpacity(float p_maxOpacity)
{
	m_header.maxOpacity = p_maxOpacity;
}
void AglParticleSystem::setOffsetType(AglParticleSystemHeader::AglSpawnOffsetType p_type)
{
	m_header.spawnOffsetType = p_type;
}
void AglParticleSystem::setSpawnType(AglParticleSystemHeader::AglSpawnType p_type)
{
	m_header.spawnType = p_type;
}
void AglParticleSystem::setParticleSize(float p_size)
{
	m_header.particleSize = p_size;
}

AglVector3 AglParticleSystem::requestSpawnPoint()
{
	AglVector3 rndDir = AglVector3(2*(((float)rand() / RAND_MAX)-0.5f), 2*(((float)rand() / RAND_MAX)-0.5f), 2*(((float)rand() / RAND_MAX)-0.5f));
	if (m_header.spawnOffsetType == AglParticleSystemHeader::ONSPHERE)
	{
		rndDir.normalize();
		return m_header.spawnPoint + rndDir*m_header.spawnOffset;
	}
	else if (m_header.spawnOffsetType == AglParticleSystemHeader::INSPHERE)
	{
		rndDir.normalize();
		float frac = (float)rand() / RAND_MAX;
		return m_header.spawnPoint + rndDir*m_header.spawnOffset*frac;
	}
	else if (m_header.spawnOffsetType == AglParticleSystemHeader::ONRING)
	{
		rndDir = rndDir - m_header.spawnDirection * AglVector3::dotProduct(m_header.spawnDirection, rndDir);
		rndDir.normalize();
		return m_header.spawnPoint + rndDir*m_header.spawnOffset;
	}
	else if (m_header.spawnOffsetType == AglParticleSystemHeader::INRING)
	{
		rndDir = rndDir - m_header.spawnDirection * AglVector3::dotProduct(m_header.spawnDirection, rndDir);
		rndDir.normalize();
		float frac = (float)rand() / RAND_MAX;
		return m_header.spawnPoint + rndDir*m_header.spawnOffset*frac;
	}
	else
		return m_header.spawnPoint;
}
AglVector3 AglParticleSystem::requestSpawnVelocity()
{
	AglVector3 rndDir = AglVector3(2*(((float)rand() / RAND_MAX)-0.5f), 2*(((float)rand() / RAND_MAX)-0.5f), 2*(((float)rand() / RAND_MAX)-0.5f));

	if (m_header.spreadType == AglParticleSystemHeader::INPLANE)
	{
		rndDir = rndDir - m_header.spawnDirection * AglVector3::dotProduct(m_header.spawnDirection, rndDir);
	}

	rndDir.normalize();
	AglVector3 dir = m_header.spawnDirection * (1.0f - m_header.spread) + rndDir * m_header.spread;
	dir.normalize();

	return dir*m_header.spawnSpeed;
}
void AglParticleSystem::setParticlesPerSpawn(unsigned int p_particlesPerSpawn)
{
	m_header.particlesPerSpawn = p_particlesPerSpawn;
}
void AglParticleSystem::setSpreadType(AglParticleSystemHeader::AglSpreadType p_type)
{
	m_header.spreadType = p_type;
}

void AglParticleSystem::restart()
{
	m_age = 0;
	m_particles.clear();
	m_timeSinceSpawn = 0;
}

void AglParticleSystem::update(float p_dt, AglVector3 p_cameraPosition)
{
	if (m_header.particleFormat == AGL_PARTICLE_FORMAT_STANDARD)
	{
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			AglStandardParticle& p = m_particles[i];
			
			p.age += p_dt;
			if (p.age > m_header.particleAge) //Kill Particle
			{
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
			else
			{
				p.position += p.velocity * p_dt;
			}
		}
		for (unsigned int i = 0; i < 5; i++)
		{
			for (unsigned int j = 1; j < m_particles.size(); j++)
			{
				//Sort
				if (AglVector3::lengthSquared(m_particles[j].position - p_cameraPosition) >
					AglVector3::lengthSquared(m_particles[j-1].position - p_cameraPosition))
				{
					AglStandardParticle temp = m_particles[j];
					m_particles[j] = m_particles[j-1];
					m_particles[j-1] = temp;
				}
			}
		}

		if (m_header.spawnType == AglParticleSystemHeader::CONTINUOUSLY)
		{
			if (m_header.spawnFrequency > 0 && m_timeSinceSpawn > 1.0f / m_header.spawnFrequency)
			{
				for (unsigned int i = 0; i < m_header.particlesPerSpawn; i++)
				{
					AglStandardParticle p(requestSpawnPoint(), requestSpawnVelocity(), m_header.particleSize);
					m_particles.push_back(p);
				}
				m_timeSinceSpawn -= (1.0f / m_header.spawnFrequency);
			}
		}
		else if (m_header.spawnType == AglParticleSystemHeader::ONCE && m_age == 0)
		{
			for (unsigned int i = 0; i < m_header.particlesPerSpawn; i++)
			{
				AglStandardParticle p(requestSpawnPoint(), requestSpawnVelocity(), m_header.particleSize);
				m_particles.push_back(p);
			}
			m_timeSinceSpawn -= (1.0f / m_header.spawnFrequency);
		}
		m_age += p_dt;
		m_timeSinceSpawn += p_dt;

	}
	else
	{
		//Do nothing
	}
}
void AglParticleSystem::setSpawnOffset(float p_offset)
{
	m_header.spawnOffset = p_offset;
}