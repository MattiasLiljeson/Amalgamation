#include "ParticleEmitters.h"

#include <AglParticleSystem.h>
#include <ParticleSystemCollection.h>

ParticleEmitters::ParticleEmitters()
{
	m_componentTypeId = ComponentType::ParticleEmitters;
}


ParticleEmitters::~ParticleEmitters()
{
}

int ParticleEmitters::addParticleSystem( const AglParticleSystem& p_system )
{
	m_collection.m_particleSystems.push_back( p_system );
	return m_collection.m_particleSystems.size()-1;
}

vector<int> ParticleEmitters::addParticleSystems( const ParticleSystemCollection& p_collection )
{
	vector<int> indicies;
	for( unsigned int i=0; i<p_collection.m_particleSystems.size(); i++ )
	{
		indicies.push_back( m_collection.m_particleSystems.size() );
		m_collection.m_particleSystems.push_back( p_collection.m_particleSystems[i] );
	}

	return indicies;
}

const ParticleSystemCollection& ParticleEmitters::getCollection()
{
	return m_collection;
}

ParticleSystemCollection* ParticleEmitters::getCollectionPtr()
{
	return &m_collection;
}

void ParticleEmitters::updateParticleSystems( const float p_dt,
											 const AglVector3& p_cameraPosition )
{
	for( unsigned int i=0; i<m_collection.m_particleSystems.size(); i++ )
	{
		m_collection.m_particleSystems[i].update( p_dt, p_cameraPosition );
	}
}
