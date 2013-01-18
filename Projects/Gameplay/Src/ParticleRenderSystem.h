#pragma once
#include <EntitySystem.h>
#include <vector>

using namespace std;

class GraphicsBackendSystem;
class AglParticleSystem;
struct AglParticleSystemHeader;
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

class ParticleRenderSystem : public EntitySystem
{
public:
	ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~ParticleRenderSystem();
	
	void process();
	//void processEntities( const vector<Entity*>& p_entities );
	unsigned int addParticleSystem(const AglParticleSystemHeader& p_header);
	unsigned int addParticleSystem();
private:
	void renderParticles(AglParticleSystem* particleSystem);
	void rebuildVertexBuffer(AglParticleSystem* particleSystem);
private:
	GraphicsBackendSystem* m_gfxBackend;
	vector<AglParticleSystem*> m_particleSystems;
};

