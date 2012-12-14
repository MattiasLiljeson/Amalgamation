#pragma once

#include <Globals.h>
#include <TcpClient.h> /* There is a problem where boost must initialize some socket-
						related stuff before other things. Therefore TcpClient should
						be included as soon as possible. Johan: Haven't looked too
						much into this yet. */

#include <EntityWorld.h>
#include <Input.h>
#include <PhysicsBody.h>
#include <BodyInitData.h>
#include <PhysicsSystem.h>
#include <RenderInfo.h>
#include <Transform.h>
#include <ShipController.h>
#include <Windows.h>
#include <AudioInfo.h>


// Systems
#include <CameraSystem.h>
#include <GraphicsBackendSystem.h>
#include <InputBackendSystem.h>
#include <PhysicsSystem.h>
#include <NetworkConnectToServerSystem.h>
#include <NetworkCommunicatorSystem.h>
#include <ProcessingMessagesSystem.h>
#include <RenderPrepSystem.h>
#include <ShipControllerSystem.h>
#include <AudioBackendSystem.h>
#include <AudioController.h>
#include <AudioListenerSystem.h>

//TEMP
#include <SoundEnums.h>

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


class ClientApplication
{
public:
	ClientApplication( HINSTANCE p_hInstance );
	~ClientApplication();

	void run();

private:
	void initSystems();
	void initEntities();
	void initSounds();
private:
	HINSTANCE m_hInstance;

	bool m_running;

	EntityWorld* m_world;
	TcpClient* m_client;
};