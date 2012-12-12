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

	It(can_contain_float_data)
	{
		Packet packet;
		float f_src = 13.37f;
		packet << f_src;
		float f_dst;
		packet >> f_dst;

		Assert::That(f_dst, Equals(f_src));
	}

	It(can_contain_char_data)
	{

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

	It(can_contain_multiple_types_of_data)
	{
		Packet packet;
		int i_src = 42;
		float f_src = 13.37f;
		packet << i_src << f_src;
		int i_dst;
		float f_dst;
		packet >> i_dst >> f_dst;

		Assert::That(i_dst, Equals(i_src));
		Assert::That(f_dst, Equals(f_src));
	}

	It(can_return_data_size)
	{
		Packet packet;

		int i_src[] = { 1, 2, 3 };
		packet << i_src[0] << i_src[1] << i_src[2];
		
		Assert::That(packet.getDataSize(), Equals(sizeof(i_src) + 1));
	}

	It(can_be_empty)
	{
		Packet packet;
		
		Assert::That(packet.isEmpty(), IsTrue());
	}

	It(throws_an_exception_if_trying_to_stream_out_an_empty_packet)
	{
		Packet packet;
		int i_dst;
		AssertThrows(std::out_of_range, (packet >> i_dst));
	}

	It(can_set_byte_data)
	{
		char* byteData = new char[5];
		// Big endian order?
		byteData[0] = 4;
		byteData[1] = 42;
		byteData[2] = 0;
		byteData[3] = 0;
		byteData[4] = 0;

		Packet packet;
		packet.setData(byteData, 5);

		int i_dst;
		packet >> i_dst;
		Assert::That(i_dst, Equals(42));

		delete[] byteData;
	}

	It(should_be_possible_to_copy_byte_data_from_a_packet_to_another)
	{
		int i_src = 5;
		float f_src = 6.9f;
		Packet packet_src;
		packet_src << i_src << f_src;

		unsigned int data_size = packet_src.getDataSize();
		char* data_src = packet_src.getDataPtr();

		Packet packet_dst;
		packet_dst.setData(data_src, data_size);

		int i_dst;
		float f_dst;
		packet_dst >> i_dst >> f_dst;

		Assert::That(i_dst, Equals(i_src));
		Assert::That(f_dst, Equals(f_src));
	}

};