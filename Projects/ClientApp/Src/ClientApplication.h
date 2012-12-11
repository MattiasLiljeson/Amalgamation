// =======================================================================================
//	ClientApplication
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief The client application run all systems related to the client logic.
///        
/// # ClientApplication
/// Detailed description...
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <TcpClient.h>

#include <CameraSystem.h>
#include <GraphicsBackendSystem.h>
#include <RenderPrepSystem.h>
#include <EntityWorld.h>
#include <Input.h>
#include <InputSystem.h>
#include <RenderInfo.h>
#include <Transform.h>
#include <Windows.h>

class ClientApplication
{
public:
	ClientApplication( HINSTANCE p_hInstance );
	~ClientApplication();

	void run();

private:
	void initSystems();
	void initEntities();

private:
	HINSTANCE m_hInstance;

	bool m_running;

	EntityWorld* m_world;
	TcpClient* m_client;

};