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

	It(can_stop_listening_by_calling_stopListening_method)
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
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		Assert::That(server.hasNewConnections(), IsTrue());
	}

	It(can_receive_connections_from_several_clients)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		Assert::That(server.hasNewConnections(), IsTrue());
	}

	It(has_no_new_packets_if_no_client_have_sent_it)
	{
		TcpServer server;
		server.startListening( 1337 );
		
		Assert::That(server.hasNewPackets(), IsFalse());
	}

	It(can_receive_a_packet_from_a_connected_client)
	{
		TcpServer server;
		server.startListening( 1337 );
	
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		client.sendPacket( new Packet( "Hello mr. Server!" ) );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		Assert::That(server.hasNewPackets(), IsTrue());
	}

	It(can_receive_several_packets_from_a_connected_client)
	{
		TcpServer server;
		server.startListening( 1337 );
	
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		client.sendPacket( new Packet( "Hello mr. Server!" ) );
		client.sendPacket( new Packet( "Hello again." ) );
		client.sendPacket( new Packet( "Blah, blah..." ) );
		boost::this_thread::sleep(boost::posix_time::millisec(50));

		server.processMessages();
		Assert::That(server.newPacketsCount(), Equals(3));
	}







	// Dummy test that previously broke the "It can receive a connection
	// from a client" test.
	It(can_lol)
	{
		Assert::That(0, Equals(0));
	}
};