#pragma once
#include <AglVector3.h>
/************************************************************************/
/* ????	-Robin & Alex													*/
/************************************************************************/
#define WIN32_LEAN_AND_MEAN // Johan: dafuq?

/************************************************************************/
/* There is a problem where boost must initialize some socket-			*/
/* related stuff before other things. Therefore TcpClient should		*/
/* be included as soon as possible. Johan: Haven't looked too			*/
/* much into this yet.													*/
/************************************************************************/
#include <Globals.h>
#include <TcpClient.h> 

#ifndef _WINDEF_
	class HINSTANCE__; // Forward or never
	typedef HINSTANCE__* HINSTANCE;
#endif

class EntityFactory;
class EntityWorld;
class TcpClient;
class GraphicsBackendSystem;

namespace Srv
{
	class ServerApplication;
};

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

	void InitModulesTestByAnton();

private:
	HINSTANCE m_hInstance;

	bool m_running;

	EntityWorld* m_world;
	TcpClient* m_client;
	GraphicsBackendSystem* m_graphicsBackendHandle;
	Srv::ServerApplication* m_serverApp;
};