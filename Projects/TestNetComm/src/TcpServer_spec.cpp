#include <igloo/igloo_alt.h>
using namespace igloo;

#include <TcpServer.h>

Describe(a_tcp_server)
{
	It(can_start_listening_on_a_port)
	{
		TcpServer server;
		server.startListening( 1337 );

		Assert::That(server.isListening(), IsTrue());
	}
};