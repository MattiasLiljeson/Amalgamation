#pragma once
#include <EntitySystem.h>
#include <RenderInterface.h>
#include <vector>
#include <ParticleSystemCollection.h>

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

class Transform;

class ParticleRenderSystem : public EntitySystem, public RenderInterface
{
public:
	ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~ParticleRenderSystem();
	
	void processEntities( const vector<Entity*>& p_entities );

	//unsigned int addParticleSystem(const AglParticleSystemHeader& p_header, int p_index);
	//AglParticleSystem* getParticleSystem(int p_index);
private:
	//void renderParticleSystem(AglParticleSystem* p_particleSystem);
	void rebuildVertexBuffer(AglParticleSystem* p_particleSystem);

	virtual void render();

private:
	GraphicsBackendSystem* m_gfxBackend;
	//vector<pair<AglParticleSystem*, int>> m_particleSystems;

	// pointer to save performance. No ownership
	vector< pair< ParticleSystemCollection*, Transform* > > m_collections;
};

