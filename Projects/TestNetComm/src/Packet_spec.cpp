#include <igloo/igloo_alt.h>
using namespace igloo;

#include <Packet.h>

Describe(a_packet)
{
	It(can_contain_int_data)
	{
		Packet packet;

		int i_src = 1;
		packet << i_src;
		int i_dst;
		packet >> i_dst;

		Assert::That(i_dst, Equals(i_src));
	}

	It(can_contain_multiple_int_data)
	{
		Packet packet;

		int i_src[] = { 1, 2, 3 };
		packet << i_src[0] << i_src[1] << i_src[2];
		int i_dst[3];
		packet >> i_dst[0] >> i_dst[1] >> i_dst[2];

		for (int i = 0; i <3; i++)
			Assert::That(i_dst[i], Equals(i_src[i]));
	}

	It(can_return_data_size)
	{
		Packet packet;

		int i_src[] = { 1, 2, 3 };
		packet << i_src[0] << i_src[1] << i_src[2];
		
		Assert::That(packet.getDataSize(), Equals(sizeof(i_src)));
	}


	It(returns_null_if_trying_to_get_byte_data_of_an_empty_packet)
	{
		Packet packet;
		
		Assert::That(packet.getDataPtr(), Is().EqualTo((char*)NULL));
	}

	It(throws_an_exception_if_trying_to_stream_out_an_empty_packet)
	{
		Packet packet;
		int i_dst;
		AssertThrows(out_of_range, packet >> i_dst);
	}

	It(can_set_byte_data)
	{
		char* byteData = new char[4];
		// Big endian order
		byteData[0] = 42;
		byteData[1] = 0;
		byteData[2] = 0;
		byteData[3] = 0;

		Packet packet;
		packet.setData(byteData, 4);

		int i_dst;
		packet >> i_dst;
		Assert::That(i_dst, Equals(42));

		delete[] byteData;
	}

	It(should_be_possible_to_copy_byte_data_from_a_packet_to_another)
	{
		int i_src[] = { 5, 10 };
		Packet packet_src;
		packet_src << (int)5 << (int)10;

		unsigned int data_size = packet_src.getDataSize();
		char* data_src = packet_src.getDataPtr();

		Packet packet_dst;
		packet_dst.setData(data_src, data_size);

		int i_dst[2];
		packet_dst >> i_dst[0] >> i_dst[1];

		Assert::That(i_dst[0], Is().EqualTo(i_src[0]));
		Assert::That(i_dst[1], Is().EqualTo(i_src[1]));
	}

};