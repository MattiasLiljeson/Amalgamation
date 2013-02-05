#include <igloo/igloo_alt.h>
using namespace igloo;

#include <TcpClient.h>
#include <TcpServer.h>

#include <boost/thread/thread.hpp>

static const int TEST_PACKET_TYPE = 100;

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
	}

	It(can_also_connect_asyncroniously)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		// Remember to also process client's process messages when using async!
		client.connectToServerAsync( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		client.processMessages();

		Assert::That(client.hasActiveConnection(), IsTrue());
	}

	It(can_see_if_there_are_new_packets)
	{
		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		Packet packet_src(TEST_PACKET_TYPE);
		packet_src << (int)42;
		server.broadcastPacket( packet_src );
		
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

		server.broadcastPacket( Packet( TEST_PACKET_TYPE ) );
		server.broadcastPacket( Packet( TEST_PACKET_TYPE ) );
		
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
		int	i_src[3] = { 42, 4, 2 };
		int	i_dst[3];

		Packet packets[3] = {TEST_PACKET_TYPE, TEST_PACKET_TYPE, TEST_PACKET_TYPE};
		for(int i=0; i<3; i++)
			packets[i] << i_src[i];


		TcpServer server;
		server.startListening( 1337 );

		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );

		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		for(int i=0; i<3; i++)
			server.broadcastPacket( packets[i] );
		
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		client.processMessages();

		Assert::That(client.newPacketsCount(), Equals(3));

		for(int i=0; i<3; i++)
		{
			Packet packet = client.popNewPacket();
			packet >> i_dst[i];
			Assert::That(i_dst[i], Equals(i_src[i]));
		}
	}

	It(can_verify_that_all_server_broadcasts_are_received)
	{
		TcpServer server;
		server.startListening( 1337 );
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		Packet packet(TEST_PACKET_TYPE);
		for(int i=0; i<50000; i++) {
			server.broadcastPacket(packet);
		}

		boost::this_thread::sleep(boost::posix_time::millisec(2000));
		client.processMessages();
//		cout << client.getTotalNumberOfOverflowPackets() * (512 / packet.getDataSize()) << endl;
		Assert::That(client.newPacketsCount(), Equals(50000));
	}

	It(can_verify_that_the_total_packets_received_is_the_same_on_higher_level_as_it_is_on_lower_level)
	{
		TcpServer server;
		server.startListening( 1337 );
		TcpClient client;
		client.connectToServer( "127.0.0.1", "1337" );
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		server.processMessages();

		Packet packet(TEST_PACKET_TYPE);
		for(int i=0; i<50000; i++) {
			server.broadcastPacket(packet);
		}
		boost::this_thread::sleep(boost::posix_time::millisec(2000));
		client.processMessages();
		client.askForCommProcessInfo();
		boost::this_thread::sleep(boost::posix_time::millisec(50));
		client.processMessages();
		Assert::That(client.newPacketsCount(),
			Equals(client.getTotalPacketsReceivedInCommProcess()));
	}
};