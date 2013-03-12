#pragma once
#include <Component.h>
#include <ParticleSystemInstructionCollection.h>
#include <vector>

class AglParticleSystem;
struct Texture;
struct AglVector3;
struct ParticleSystemAndTexture;
struct ParticleSystemInstructionCollection;

class ParticleSystemsComponent : public Component
{
public:
	ParticleSystemsComponent();
	~ParticleSystemsComponent();

	/** Adds particles systems. \return Returns the particle systems indices in the component*/
	vector<int> addParticleSystemInstructions( 
		const ParticleSystemInstructionCollection& p_system );
	/** Adds a particle system. \return Returns the particle systems index in the component*/
	int addParticleSystemInstruction( const ParticleSystemInstruction& p_instruction, int p_idx = -1 );
	bool removeParticleSystemInstruction( int p_idx );

	int addParticleSystem( const AglParticleSystem& p_system, int p_textureIdx, int p_idx = -1 );
	int addParticleSystem( const ParticleSystemAndTexture& p_system, int p_idx = -1  );
	bool removeParticleSystem( int p_idx );

	int handleIdxRequest( int p_idx );

	void updateParticleSystems( const float p_dt, const AglVector3& p_cameraPosition );
	void setSpawn( const AglMatrix& p_base );
	void setSpawn( const AglMatrix& p_base, int p_index );
	void setScale( const AglVector2& p_scale );

	ParticleSystemInstruction* getParticleSystemInstruction( int p_idx );
	AglParticleSystem* getParticleSystemPtr( int p_idx );
	ParticleSystemAndTexture* getParticleSystemAndTexturePtr( int p_idx );

	const vector< pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> >& getParticleSystems();
	vector< pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> >* getParticleSystemsPtr();

	int getInstructionCnt(); ///<- Instructions in the vector that aren't NULL
	int getParticleSystemCnt(); ///<- Particle Systems in the vector that aren't NULL

private:
	vector< pair<ParticleSystemInstruction*, ParticleSystemAndTexture*> > m_particleSystems;
	int m_instructionCnt;
	int m_particleSystemCnt;
};

