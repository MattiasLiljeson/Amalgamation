#pragma once
#include <Component.h>
#include <ParticleSystemCollection.h>
#include <vector>

struct AglVector3;
class AglParticleSystem;
struct ParticleSystemCollection;

class ParticleEmitters : public Component
{
public:
	ParticleEmitters();
	~ParticleEmitters();

	/** Adds a particle system. \return Returns the particle systems index in the component*/
	int addParticleSystem( const AglParticleSystem& p_system );
	
	/** Adds particles systems. \return Returns the particle systems indices in the component*/
	vector<int> addParticleSystems( const ParticleSystemCollection& p_system );

	void updateParticleSystems( const float p_dt, const AglVector3& p_cameraPosition );

	const ParticleSystemCollection& getCollection();
	ParticleSystemCollection* getCollectionPtr();
	
private:
	ParticleSystemCollection m_collection;

};

