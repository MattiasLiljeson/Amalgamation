#include <igloo/igloo_alt.h>
using namespace igloo;

#include <TcpClient.h>
#include <TcpServer.h>

Describe(a_tcp_client)
{
	It(can_have_an_active_connection_if_it_has_connected)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "localhost", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(500));
		server.hasNewConnections();

		Assert::That(client.hasActiveConnection(), IsTrue());
	}

	It(has_no_active_connection_if_it_has_not_connected) // duh! :)
	{
		TcpClient client;
		Assert::That(client.hasActiveConnection(), IsFalse());
	}
};