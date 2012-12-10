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
		client.connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		Assert::That(client.hasActiveConnection(), IsTrue());
	}

	It(has_no_active_connection_if_it_has_not_connected) // duh! :)
	{
		TcpClient client;
		Assert::That(client.hasActiveConnection(), IsFalse());
	}

	It(can_disconnect_from_server)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		client.disconnect();

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		Assert::That(client.hasActiveConnection(), IsFalse());
		Assert::That(server.activeConnectionsCount(), Equals(0));
	}

	It(can_see_if_there_are_new_packets)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		server.broadcastPacket( new Packet( "This is a broadcast message!" ) );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		client.processMessages();

		Assert::That(client.hasNewPackets(), IsTrue());
	}

	It(can_see_how_many_new_packets_there_are)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		server.broadcastPacket( new Packet( "This is a broadcast message!" ) );
		server.broadcastPacket( new Packet( "This is another broadcast message!" ) );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		client.processMessages();

		Assert::That(client.newPacketsCount(), Equals(2));
	}

	It(can_see_if_there_are_no_new_received_packets)
	{
		TcpClient client;
		Assert::That(client.hasNewPackets(), IsFalse());
	}

	It(returns_zero_if_newPacketsCount_is_called_and_there_are_no_new_packets)
	{
		TcpClient client;
		Assert::That(client.newPacketsCount(), Equals(0));
	}

	It(can_read_the_same_packets_that_are_sent)
	{
		string messages[] =
		{
			"This is a broadcast message!",
			"This is another broadcast message!",
			"This is yet another broadcast message!"
		};

		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		for(int i=0; i<3; i++)
			server.broadcastPacket( new Packet( messages[i] ) );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		client.processMessages();

		Assert::That(client.newPacketsCount(), Equals(3));
		for(int i=0; i<3; i++)
		{
			Packet* packet = client.popNewPacket();
			Assert::That(packet->getMessage(), Equals(messages[i]));
			delete packet;
		}
	}

};