#pragma once

#include <AntTweakBarWrapper.h>
#include <EntitySystem.h>
#include <Windows.h>


class AglParticleSystem;
class GraphicsWrapper;
class Window;
struct AglMeshHeader;
struct AglVector2;
struct ConnectionPointCollection;
struct InstanceData;
struct ParticleSystemAndTexture;
struct RendererSceneInfo;
// =======================================================================================
//                                      GraphicsBackendSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # GraphicsBackendSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class GraphicsBackendSystem : public EntitySystem
{
public:
	GraphicsBackendSystem( HINSTANCE p_hInstance, int p_scrWidth = 1280, 
		int p_scrHeight = 720, bool p_windowed = true );
	~GraphicsBackendSystem(void);

	void changeResolution( int p_scrWidth, int p_scrHeight );

	virtual void initialize();
	void process();

	// vector<Entity*> buildEntitiesFromMeshFile( const string& p_meshName, const string* p_path=NULL);
	// int				loadSingleMesh( const string& p_meshName, const string* p_path=NULL);

	int getMeshId( const string& p_meshName );
	GraphicsWrapper* getGfxWrapper();
	HWND getWindowRef();
	float getAspectRatio();
	AglVector2 getWindowSize();

	void renderParticleSystem( ParticleSystemAndTexture* p_system,
		const InstanceData& p_worldTransform );
private:
	GraphicsWrapper* m_graphicsWrapper;

	HINSTANCE m_hInstance;
	Window* m_window;

	int m_scrWidth;
	int m_scrHeight;
	bool m_windowed;
	bool m_wireframe;
	/************************************************************************/
	/* DEBUG FUNCTIONS ONLY! */
	/************************************************************************/
	static GraphicsBackendSystem* m_selfPointer;

	int m_newWidth;
	int m_newHeight;
private:
	static void TW_CALL toggleFullScreen(void* p_clientData);
	static void TW_CALL toggleWireframe(void* p_clientData);
	static void TW_CALL applyNewResolution(void* p_clientData);
};

