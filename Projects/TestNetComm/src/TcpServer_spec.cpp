#include <igloo/igloo_alt.h>
using namespace igloo;

#include <boost/thread.hpp>

#include <TcpServer.h>
#include <TcpClient.h>

Describe(a_tcp_server)
{
	It(can_start_listening_on_a_port)
	{
		TcpServer server;
		server.startListening( 1337 );

		Assert::That(server.isListening(), IsTrue());
	}

	It(can_stop_listening_by_calling_stopListening)
	{
		TcpServer server;
		server.startListening( 1337 );
		server.stopListening();

		Assert::That(server.isListening(), IsFalse());
	}

	It(has_no_connections_if_no_client_connected)
	{
		TcpServer server;

		Assert::That(server.hasNewConnections(), IsFalse());
	}

	It(can_receive_a_connection_from_a_client)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "localhost", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(1000));

		Assert::That(server.hasNewConnections(), IsTrue());
	}
};