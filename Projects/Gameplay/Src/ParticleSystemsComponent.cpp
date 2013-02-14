#include "ParticleSystemsComponent.h"

#include <AglParticleSystem.h>
#include <ParticleSystemInstructionCollection.h>
#include <GraphicsWrapper.h>
#include <Globals.h>
#include <ParticleSystemAndTexture.h>

ParticleSystemsComponent::ParticleSystemsComponent()
	: Component( ComponentType::ParticleSystemsComponent )
{
	m_componentTypeId = ComponentType::ParticleSystemsComponent;
	m_instructionCnt = 0;
	m_particleSystemCnt = 0;
}


ParticleSystemsComponent::~ParticleSystemsComponent()
{
	for( unsigned int i=0; i<m_particleSystems.size(); i++ ) {
		delete m_particleSystems[i].first;
		m_particleSystems[i].first = NULL;
		delete m_particleSystems[i].second;
		m_particleSystems[i].second = NULL;
	}
}

vector<int> ParticleSystemsComponent::addParticleSystemInstructions(
	const ParticleSystemInstructionCollection& p_collection )
{
	vector<int> indicies;
	for( unsigned int i=0; i<p_collection.m_collection.size(); i++ ) {
		indicies.push_back( addParticleSystemInstruction( p_collection.m_collection[i] ) );
	}
	return indicies;
}

int ParticleSystemsComponent::addParticleSystemInstruction(
	const ParticleSystemInstruction& p_instruction, int p_idx )
{
	int newIdx = handleIdxRequest( p_idx );

	pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> ps;
	ps.first =  new ParticleSystemInstruction( p_instruction );
	ps.second = NULL;
	m_particleSystems[newIdx] = ps;
	m_instructionCnt++;
	return m_particleSystems.size() -1;
}

bool ParticleSystemsComponent::removeParticleSystemInstruction( int p_idx )
{
	if( 0<= p_idx && p_idx < m_particleSystems.size() ){
		delete m_particleSystems[p_idx].first;
		m_particleSystems[p_idx].first = NULL;
		m_instructionCnt--;
		return true;
	}
	return false;
}

int ParticleSystemsComponent::addParticleSystem( const AglParticleSystem& p_system,
	int p_textureIdx, int p_idx )
{
	ParticleSystemAndTexture ps = { p_system, p_textureIdx };
	return addParticleSystem( ps, p_idx );
}

int ParticleSystemsComponent::addParticleSystem( const ParticleSystemAndTexture& p_system,
	int p_idx )
{
	int newIdx = handleIdxRequest( p_idx );
	pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> ps;
	ps.first = NULL;
	ps.second =  new ParticleSystemAndTexture( p_system );
	m_particleSystems[newIdx] = ps;
	m_particleSystemCnt++;
	return m_particleSystems.size() -1;
}

bool ParticleSystemsComponent::removeParticleSystem( int p_idx )
{
	if( 0<= p_idx && p_idx < m_particleSystems.size() ) {
		delete m_particleSystems[p_idx].second;
		m_particleSystems[p_idx].second = NULL;
		m_particleSystemCnt--;
		return true;
	}
	return false;
}

int ParticleSystemsComponent::handleIdxRequest( int p_idx )
{
	int newIdx = -1;

	// if idx not specified
	if ( p_idx == -1 ) {
		// Make room for a new pair
		newIdx = m_particleSystems.size();
		m_particleSystems.resize( newIdx+1 );
	} 
	// If idx in the middle of the vector
	else if( 0<= p_idx && p_idx < m_particleSystems.size() ) {
		newIdx = p_idx;
		// If the place is already occupied
		if( m_particleSystems[p_idx].first != NULL ||
			m_particleSystems[p_idx].second != NULL )
		{
			// Move the existing pair to the back
			m_particleSystems.push_back(m_particleSystems[newIdx]);
		}	
	} 
	// If idx outside of the vector range.
	else {
		// Expand the vector so the given idx can be used;
		newIdx = p_idx;
		m_particleSystems.resize( newIdx+1 );
	}

	// The newly made room must be set to NULL if both parts of the pair isn't used.
	m_particleSystems[newIdx].first = NULL;
	m_particleSystems[newIdx].second = NULL;
	return newIdx;
}

void ParticleSystemsComponent::updateParticleSystems( const float p_dt,
											 const AglVector3& p_cameraPosition )
{
	for( unsigned int i=0; i<m_particleSystems.size(); i++ ) {
		if( m_particleSystems[i].second != NULL ) {
			AglParticleSystem* ps = &m_particleSystems[i].second->particleSystem;
			ps->update( p_dt, p_cameraPosition );
		}
	}
}

void ParticleSystemsComponent::setSpawn( const AglVector3& p_spawnPoint,
								const AglVector3& p_spawnDirection )
{
	for( unsigned int i=0; i<m_particleSystems.size(); i++ ) {
		if( m_particleSystems[i].second != NULL ) {
			AglParticleSystem* ps = &m_particleSystems[i].second->particleSystem;
			ps->setSpawnPoint( p_spawnPoint );
			AglVector3 dir = ps->getSpawnDirection();
			AglVector3 newDir = dir + p_spawnPoint;
			ps->setCurrSpawnDirection( newDir );
		}
	}
}

const vector< pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> >& ParticleSystemsComponent::getParticleSystems()
{
	return m_particleSystems;
}

vector< pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> >* ParticleSystemsComponent::getParticleSystemsPtr()
{
	return &m_particleSystems;
}

int ParticleSystemsComponent::getInstructionCnt()
{
	return m_instructionCnt;
}

int ParticleSystemsComponent::getParticleSystemCnt()
{
	return m_particleSystemCnt;
}

ParticleSystemInstruction* ParticleSystemsComponent::getParticleSystemInstruction( int p_idx )
{
	ParticleSystemInstruction* instruction = NULL;
	if( 0<= p_idx && p_idx < m_particleSystems.size() ) {
		instruction = m_particleSystems[p_idx].first;
	}
	return instruction;
}

AglParticleSystem* ParticleSystemsComponent::getParticleSystemPtr( int p_idx )
{
	AglParticleSystem* ps = NULL;
	if( 0<= p_idx && p_idx < m_particleSystems.size() ) {
		ps = &m_particleSystems[p_idx].second->particleSystem;
	}
	return ps;
}

ParticleSystemAndTexture* ParticleSystemsComponent::getParticleSystemAndTexturePtr( int p_idx )
{
	ParticleSystemAndTexture* psAndTex = NULL;
	if( 0<= p_idx && p_idx < m_particleSystems.size() ) {
		psAndTex = m_particleSystems[p_idx].second;
	}
	return psAndTex;
}