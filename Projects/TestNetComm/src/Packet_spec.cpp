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
	
	It(can_return_byte_data)
	{


	}

	It(can_return_data_size)
	{

	}

	It(can_set_byte_data)
	{

	}
};