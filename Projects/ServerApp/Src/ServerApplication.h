#pragma once

#include <conio.h>
#include <EntityWorld.h>
#include <ProcessThread.h>

class TcpServer;

using namespace std;
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

namespace Srv
{
	class ServerApplication : public ProcessThread
	{
	public:
		ServerApplication(int p_gameTime=600,int p_activePort=1337, 
			const string& p_name="monki");
		virtual ~ServerApplication();

		///-----------------------------------------------------------------------------------
		/// Simply calls the body function.
		/// \return void
		///-----------------------------------------------------------------------------------
		void run();

		///-----------------------------------------------------------------------------------
		/// Call one "step" on the server, instead of running a game loop.
		/// \return void
		///-----------------------------------------------------------------------------------
		void step( float p_dt );

		///-----------------------------------------------------------------------------------
		/// The body() method starts the whole application. Need for the server application to
		/// be able to implement the ProcessThread.
		/// \return void
		///-----------------------------------------------------------------------------------
		void body();

		///-----------------------------------------------------------------------------------
		/// Empty implemenation.
		/// \return void
		///-----------------------------------------------------------------------------------
		void processMessages();

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

		void InitModulesTestByAnton();

	private:
		bool m_running;

		EntityWorld* m_world;
		TcpServer* m_server;
		int m_activePort;
		int m_gameTime;
		string m_serverName;
	};
};