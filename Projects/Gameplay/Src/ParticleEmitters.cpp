#include "ParticleEmitters.h"

#include <AglParticleSystem.h>
#include <ParticleSystemCollection.h>

ParticleEmitters::ParticleEmitters()
	: Component( ComponentType::ParticleEmitters )
{
	m_componentTypeId = ComponentType::ParticleEmitters;
}


ParticleEmitters::~ParticleEmitters()
{
}

int ParticleEmitters::addParticleSystem( const AglParticleSystem& p_system )
{
	ParticleSystemInstruction psi = {p_system,"textureHere"};
	m_collection.m_collection.push_back( psi );
	return m_collection.m_collection.size()-1;
}

vector<int> ParticleEmitters::addParticleSystems( const ParticleSystemCollection& p_collection )
{
	vector<int> indicies;
	for( unsigned int i=0; i<p_collection.m_collection.size(); i++ )
	{
		indicies.push_back( m_collection.m_collection.size() );
		m_collection.m_collection.push_back( p_collection.m_collection[i] );
	}

	return indicies;
}

void ParticleEmitters::updateParticleSystems( const float p_dt,
											 const AglVector3& p_cameraPosition )
{
	for( unsigned int i=0; i<m_collection.m_collection.size(); i++ )
	{
		m_collection.m_collection[i].particleSystem.update( p_dt, p_cameraPosition );
	}
}

void ParticleEmitters::setSpawn( const AglVector3& p_spawnPoint,
								const AglVector3& p_spawnDirection )
{
	for( unsigned int i=0; i<m_collection.m_collection.size(); i++ )
	{
		m_collection.m_collection[i].particleSystem.setSpawnPoint( p_spawnPoint );
		AglVector3 dir = m_collection.m_collection[i].particleSystem.getSpawnDirection();
		AglVector3 newDir = dir + p_spawnPoint;
		m_collection.m_collection[i].particleSystem.setCurrSpawnDirection( newDir );
	}
}

const ParticleSystemCollection& ParticleEmitters::getCollection()
{
	return m_collection;
}

ParticleSystemCollection* ParticleEmitters::getCollectionPtr()
{
	return &m_collection;
}
