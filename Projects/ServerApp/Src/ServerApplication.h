// =======================================================================================
//                                      ServerApplication
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	The server application run all systems related to the server logic.
/// 
/// # ServerApplication
/// The server application contains its own EntityWorld. It consists of different
/// ES-systems which in turn are containing game logic, physics, network logic etc...
/// It also calls the EntityWorld's update (or rather "tick") method.
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <conio.h>

#include <boost/thread.hpp>

#include <EntityWorld.h>
#include <PrintPositionsSystem.h>
#include <ProcessingMessagesSystem.h>
#include <NetworkListenerSystem.h>
#include <NetworkInputHandlerSystem.h>
#include <NetworkUpdateSystem.h>
#include <TcpServer.h>

using namespace std;

class ServerApplication
{
public:
	ServerApplication();
	virtual ~ServerApplication();

	///-----------------------------------------------------------------------------------
	/// The run() method starts the whole application. It runs all the
	/// Let the program's entry point call run().
	/// \return void
	///-----------------------------------------------------------------------------------
	void run();

private:

	///-----------------------------------------------------------------------------------
	/// Creates and initializes all EntitySystems.
	/// \return void
	///-----------------------------------------------------------------------------------
	void initSystems();
	
	///-----------------------------------------------------------------------------------
	/// Creates and adds entities to the world.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void initEntities();

private:
	bool m_running;

	EntityWorld* m_world;
	TcpServer* m_server;

};