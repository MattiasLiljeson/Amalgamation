#pragma once

#include <AglMatrix.h>
#include <AglVector3.h>
#include <DebugUtil.h>
#include <EntitySystem.h>
#include <GraphicsWrapper.h>
#include <TextureParser.h>
#include <Window.h>
#include <Windows.h>
#include <vld.h>


class RenderPrepSystem : public EntitySystem
{
public:
	RenderPrepSystem( HINSTANCE p_hInstance, int p_scrWidth = 800, int p_scrHeight = 600,
		bool p_windowed = true);
	virtual ~RenderPrepSystem();
	virtual void initialize();

	virtual void process();

private:
	HINSTANCE m_hInstance;
	Window* m_window;
	GraphicsWrapper* m_graphicsWrapper;

	// simple timer
	__int64 m_cntsPerSec;
	__int64 m_prevTimeStamp;
	float m_secsPerCnt;
	float m_ticker;

	// Temp cube object
	unsigned int m_cubeId;
	AglMatrix m_viewMatrix;
	AglMatrix m_projMatrix;
	AglMatrix m_camMatrix; 

	AglVector3 m_pos;
	AglVector3 m_lookAt;
	AglVector3 m_up;

	RendererSceneInfo m_sceneInfo;

	int m_scrWidth;
	int m_scrHeight;
	bool m_windowed;

};

