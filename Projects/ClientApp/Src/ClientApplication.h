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

#include <TcpClient.h> /* There is a problem where boost must initialize some socket-
						related stuff before other things. Therefore TcpClient should
						be included as soon as possible. Johan: Haven't looked too
						much into this yet. */

#include <EntityWorld.h>
#include <Input.h>
#include <Windows.h>
#include <AntTweakBarWrapper.h>

// Components
#include <PhysicsBody.h>
#include <RenderInfo.h>
#include <Transform.h>
#include <Identity.h>

// Systems
#include <InputSystem.h>
#include <PhysicsSystem.h>
#include <CameraSystem.h>
#include <GraphicsBackendSystem.h>
#include <InputSystem.h>
#include <NetworkConnectToServerSystem.h>
#include <NetworkCommunicatorSystem.h>
#include <ProcessingMessagesSystem.h>
#include <RenderPrepSystem.h>

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