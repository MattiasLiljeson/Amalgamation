#pragma once

#include <DebugUtil.h>
#include <EntitySystem.h>
#include <GraphicsWrapper.h>
#include <Window.h>
#include <Windows.h>
#include <SystemType.h>
#include <ComponentType.h>

class GraphicsBackendSystem : public EntitySystem
{
public:
	GraphicsBackendSystem( HINSTANCE p_hInstance, int p_scrWidth = 800, 
		int p_scrHeight = 600, bool p_windowed = true );
	~GraphicsBackendSystem(void);

	virtual void initialize();
	void process();

	int getMeshId( string p_meshName );
	GraphicsWrapper* getGfxWrapper();
	HWND getWindowRef();

private:
	GraphicsWrapper* m_graphicsWrapper;

	HINSTANCE m_hInstance;
	Window* m_window;

	int m_scrWidth;
	int m_scrHeight;
	bool m_windowed;
};

