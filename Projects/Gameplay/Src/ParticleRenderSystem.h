#pragma once
#include <EntitySystem.h>
#include <RenderInterface.h>
#include <vector>
#include <ParticleSystemInstructionCollection.h>

using namespace std;

class GraphicsBackendSystem;
class AglParticleSystem;
struct AglParticleSystemHeader;
struct ParticleSystemAndTexture;
// =======================================================================================
//                                      ParticleRenderSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ParticleRenderSystem
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class Transform;

class ParticleRenderSystem : public EntitySystem, public RenderInterface
{
public:
	ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~ParticleRenderSystem();
	
	void processEntities( const vector<Entity*>& p_entities );

	virtual void render();
	virtual void renderAdditiveParticles();
	virtual void renderAlphaParticles();
	virtual void renderMultiplyParticles();

private:
	//void renderParticleSystem(AglParticleSystem* p_particleSystem);
	void rebuildVertexBuffer(AglParticleSystem* p_particleSystem);

private:
	GraphicsBackendSystem* m_gfxBackend;
	//vector<pair<AglParticleSystem*, int>> m_particleSystems;

	// pointer to save performance. No ownership
	vector< pair< ParticleSystemAndTexture*, Transform* > > m_additiveCollection;
	vector< pair< ParticleSystemAndTexture*, Transform* > > m_alphaCollection;
	vector< pair< ParticleSystemAndTexture*, Transform* > > m_multiplyCollection;
};

