#pragma once

#include <AntTweakBarWrapper.h>
#include <EntitySystem.h>
#include <Windows.h>


class Window;
class GraphicsWrapper;
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
	GraphicsBackendSystem( HINSTANCE p_hInstance, int p_scrWidth = 800, 
		int p_scrHeight = 600, bool p_windowed = true );
	~GraphicsBackendSystem(void);

	virtual void initialize();
	void process();

	unsigned int createMesh( const string& p_meshName,
							 const string* p_path=NULL );
	int getMeshId( const string& p_meshName );
	GraphicsWrapper* getGfxWrapper();
	HWND getWindowRef();

private:
	GraphicsWrapper* m_graphicsWrapper;

	HINSTANCE m_hInstance;
	Window* m_window;

	int m_scrWidth;
	int m_scrHeight;
	bool m_windowed;
	/************************************************************************/
	/* DEBUG FUNCTIONS ONLY! */
	/************************************************************************/
	static GraphicsBackendSystem* m_selfPointer;
private:
	static void TW_CALL toggleFullScreen(void* p_clientData);
};

