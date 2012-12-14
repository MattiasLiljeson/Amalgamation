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
		
		Packet packet;
		packet << (int)0;

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		client.sendPacket( packet );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		Assert::That(server.hasNewPackets(), IsTrue());
	}

	It(can_receive_several_packets_from_a_connected_client)
	{
		Packet packets[3];
		for (int i = 0; i < 3; i++)
			packets[i] << i;

		TcpServer server;
		server.startListening( 1337 );
	
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		for( unsigned int i=0; i<3; i++ )
			client.sendPacket( packets[i] );
		boost::this_thread::sleep(boost::posix_time::millisec(50));

		server.processMessages();

		Assert::That(server.newPacketsCount(), Equals(3));
	}

	It(can_return_newly_received_packets)
	{

		Packet packets[3];
		for (int i = 0; i < 3; i++)
			packets[i] << i;

		TcpServer server;
		server.startListening( 1337 );
	
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		for( unsigned int i=0; i<3; i++ )
			client.sendPacket( packets[i] );
		boost::this_thread::sleep(boost::posix_time::millisec(50));

		server.processMessages();

		Assert::That(server.newPacketsCount(), Equals(3));
		for( unsigned int i=0; i<3; i++ )
		{
			Packet packet = server.popNewPacket();
			int data;
			packet >> data;
			Assert::That(data, Equals(i));
		}
	}

	It(should_cast_a_domain_exception_when_popping_an_empty_packet_queue)
	{
		TcpServer server;
		
		AssertThrows(std::domain_error, server.popNewPacket());
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
		TcpServer server;
		server.startListening( 1337 );
		
		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		
		int i_src = 32;
		Packet packet_src;
		packet_src << i_src;
		server.broadcastPacket( packet_src );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		for(int i=0; i<5; i++)
			client[i].processMessages();

		for(int i=0; i<5; i++)
		{
			Assert::That(client[i].newPacketsCount(), Equals(1));
			Packet packet_dst = client[i].popNewPacket();
			int i_dst;
			packet_dst >> i_dst;

			Assert::That(i_dst, Equals(i_src));
		}
	}

	It(can_multicast_packets_to_a_list_of_clients)
	{
		TcpServer server;
		server.startListening( 1337 );
		
		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		
		int i_src = 32;
		Packet packet_src;
		packet_src << i_src;

		vector<int> connections;
		connections = server.getActiveConnections();
		connections.erase( connections.begin() );
		server.multicastPacket( connections, packet_src );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		for(int i=0; i<5; i++)
			client[i].processMessages();

		int numberOfClientsThatReceivedPacket = 0;
		for(int i=0; i<5; i++)
		{
			if( client[i].newPacketsCount() > 0 )
			{
				numberOfClientsThatReceivedPacket += 1;
			}
		}

		Assert::That(numberOfClientsThatReceivedPacket, Equals(4));
	}

	It(can_unicast_packets_to_individual_clients)
	{
		TcpServer server;
		server.startListening( 1337 );
		
		TcpClient client[3];
		for(int i=0; i<3; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		
		vector<int> currentConnections = server.getActiveConnections();

		Packet packets[3];
		for (int i = 0; i < 3; i++)
		{
			packets[i] << i + 111;
			server.unicastPacket( packets[i], currentConnections[i] );
		}

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		for(int i=0; i<3; i++)
		{
			client[i].processMessages();
			
			Assert::That(client[i].hasNewPackets(), IsTrue());
			Packet packet = client[i].popNewPacket();
			int i_dst;
			packet >> i_dst;
			Assert::That(i_dst, IsGreaterThan(110)); /** Remember the values set to be
														i + 111 (greater than 110). */
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

	It(can_return_all_active_connections_as_a_vector)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client[5];
		for(int i=0; i<5; i++)
			client[i].connectToServer( "127.0.0.1", "1337" );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		vector< int > currentConnections;
		currentConnections = server.getActiveConnections();

		Assert::That(currentConnections.size(), Equals(5));
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