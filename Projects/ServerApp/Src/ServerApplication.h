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

#include <EntityWorld.h>
#include <NetworkListenerSystem.h>

using namespace std;

class ServerApplication
{
public:
	ServerApplication();
	virtual ~ServerApplication();

	///-----------------------------------------------------------------------------------
	/// The run() method starts the whole application. It runs all the
	/// Let the program's entry point call run().
	/// \returns void
	///-----------------------------------------------------------------------------------
	void run();

private:

	///-----------------------------------------------------------------------------------
	/// Creates and initializes all EntitySystems.
	/// \returns void
	///-----------------------------------------------------------------------------------
	void initSystems();
	
	/// ... Add more ES-initializations below.

private:
	bool m_running;

	EntityWorld* m_world;

};