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

	It(can_tell_how_many_new_connections_there_are)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		Assert::That(server.newConnectionsCount(), Equals(5));
	}

	It(can_tell_how_many_active_connections_there_are)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		Assert::That(server.activeConnectionsCount(), Equals(5));
	}

	It(can_tell_how_many_new_disconnections_there_are)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		for(int i=0; i<5; i++)
			client[i].disconnect();
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		Assert::That(server.newDisconnectionsCount(), Equals(5));
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
		string packetStrings[] =
		{
			"Hello mr. Server!",
			"Hello again.",
			"Blah, blah..."
		};

		TcpServer server;
		server.startListening( 1337 );
	
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		for( unsigned int i=0; i<3; i++ )
			client.sendPacket( new Packet( packetStrings[i] ) );
		boost::this_thread::sleep(boost::posix_time::millisec(50));

		server.processMessages();
		Assert::That(server.newPacketsCount(), Equals(3));
	}

	It(can_return_newly_received_packets)
	{
		string packetStrings[] =
		{
			"Hello mr. Server!",
			"Hello again.",
			"Blah, blah..."
		};

		TcpServer server;
		server.startListening( 1337 );
	
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		for( unsigned int i=0; i<3; i++ )
			client.sendPacket( new Packet( packetStrings[i] ) );
		boost::this_thread::sleep(boost::posix_time::millisec(50));

		server.processMessages();

		Packet* packet = NULL;
		Assert::That(server.newPacketsCount(), Equals(3));
		for( unsigned int i=0; i<3; i++ )
		{
			packet = server.popNewPacket();
			Assert::That(packet->getMessage(), Equals(packetStrings[i]));
			delete packet;
		}
	}

	It(should_return_null_when_popping_an_empty_packet_queue)
	{
		TcpServer server;
		
		Assert::That( server.popNewPacket(), Equals((Packet*)NULL) );
	}

	It(has_an_interface_that_can_be_used_to_process_messages)
	{
		ThreadSafeMessaging* server = new TcpServer();

		server->processMessages();

		delete server;
	}

	It(can_get_the_id_of_a_new_connection)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		if( server.hasNewConnections() )
		{
			// Id's start at 0.
			Assert::That(server.popNewConnection(), IsGreaterThan(-1));
		}
	}

	It(should_return_minus_one_if_trying_to_pop_an_empty_new_connections_queue)
	{
		TcpServer server;

		Assert::That(server.popNewConnection(), Equals(-1));
	}

	It(can_broadcast_packets_to_all_connected_clients)
	{
		string messages[] =
		{
			"This is a broadcast message!"
		};

		TcpServer server;
		server.startListening( 1337 );
		
		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
			
		server.broadcastPacket( new Packet( messages[0] ) );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		for(int i=0; i<5; i++)
			client[i].processMessages();

		for(int i=0; i<5; i++)
		{
			Assert::That(client[i].newPacketsCount(), Equals(1));
			Packet* packet = client[i].popNewPacket();
			Assert::That(packet->getMessage(), Equals(messages[0]));
			delete packet;
		}
	}

	It(has_no_active_connection_if_the_only_client_has_disconnected)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		client.disconnect();

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		//boost::this_thread::sleep(boost::posix_time::millisec(50));
		//server.processMessages();
		//boost::this_thread::sleep(boost::posix_time::millisec(50));
		//server.processMessages();


		Assert::That(server.activeConnectionsCount(), Equals(0));
	}

//	It(can_see_a_client_disconnecting)
//	{
//		TcpServer server;
//		server.startListening( 1337 );
//	
//		TcpClient client;
//		client.connectToServer( "127.0.0.1", "1337" );
//		client.disconnect();
//		
//		boost::this_thread::sleep(boost::posix_time::millisec(50));
//		server.processMessages();
//
//		Assert::That(server.hasNewDisconnections(), 
//	}

	// Dummy test that previously broke the "It can receive a connection
	// from a client" test.
	It(can_lol)
	{
		Assert::That(0, Equals(0));
	}
};