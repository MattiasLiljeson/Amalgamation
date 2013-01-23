#pragma once
#include <EntitySystem.h>
#include <RenderInterface.h>
#include <vector>

using namespace std;

class GraphicsBackendSystem;
class LightMesh;
struct LightInstanceData;

// =======================================================================================
//                                      LightRenderSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LightRenderSystem
/// Detailed description.....
/// Created on: 22-1-2013 
///---------------------------------------------------------------------------------------

class LightRenderSystem : public EntitySystem, public RenderInterface
{
public:
	LightRenderSystem( GraphicsBackendSystem* p_gfxBackend );
	~LightRenderSystem();

	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void initialize();

	virtual void render();
private:
	GraphicsBackendSystem* m_gfxBackend;
	LightMesh* m_box;
	vector<LightInstanceData> m_data;
};

