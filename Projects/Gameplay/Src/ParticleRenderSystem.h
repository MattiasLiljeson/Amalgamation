#pragma once
#include <EntitySystem.h>
#include <RenderInterface.h>
#include <vector>
#include <ParticleSystemInstructionCollection.h>
#include <RenderStateEnums.h>
#include <InstanceData.h>

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

struct PsRenderInfo
{
	// Pointers to save performance. No ownership
	ParticleSystemAndTexture* psAndTex;
	InstanceData transform;
	AglParticleSystemHeader::AglBlendMode blendMode;
	AglParticleSystemHeader::AglRasterizerMode rasterizerMode;

	PsRenderInfo( ParticleSystemAndTexture* p_psAndTex, InstanceData p_transform );
};

class ParticleRenderSystem : public EntitySystem, public RenderInterface
{
public:
	ParticleRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~ParticleRenderSystem();
	
	void processEntities( const vector<Entity*>& p_entities );

	virtual void render();
	virtual void render( AglParticleSystemHeader::AglBlendMode p_blendMode,
		AglParticleSystemHeader::AglRasterizerMode p_rasterizerMode );

	virtual BlendState::Mode blendStateFromAglBlendMode(
		AglParticleSystemHeader::AglBlendMode p_blend );
	virtual RasterizerState::Mode rasterizerStateFromAglRasterizerMode(
		AglParticleSystemHeader::AglRasterizerMode p_rast);

	int* getDrawnPSCount()
	{
		return &drawnPS;
	}
	int* getMaxDrawnPSCount()
	{
		return &maxDrawnPS;
	}

private:
	//void renderParticleSystem(AglParticleSystem* p_particleSystem);
	//void rebuildVertexBuffer( AglParticleSystem* p_particleSystem );
	bool insertToRenderQue( PsRenderInfo p_renderInfo );
	void clearRenderQues();

	//Should be moved to a culling system perhaps? //AA
	bool shouldRender(ParticleSystemAndTexture* p_ps);
	bool boxPlane(const AglVector3& p_min, const AglVector3& p_max, const AglVector4& p_plane);
	void calcCameraPlanes();

private:
	GraphicsBackendSystem* m_gfxBackend;
	
	vector< PsRenderInfo > m_renderQues
		[AglParticleSystemHeader::AglBlendMode_CNT]
		[AglParticleSystemHeader::AglRasterizerMode_CNT];


	//Culling system? //AA
	int drawnPS;
	int maxDrawnPS;

	AglVector4 m_cameraPlanes[6];

	float timeInGame;
};

