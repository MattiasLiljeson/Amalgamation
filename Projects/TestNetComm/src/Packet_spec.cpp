#include <igloo/igloo_alt.h>
using namespace igloo;

#include <Packet.h>

Describe(a_packet)
{
	It(can_contain_an_empty_message)
	{
		Packet packet;
		Assert::That(packet.getMessage(), Equals(""));
	}

	It(can_contain_a_string_as_a_message)
	{
		Packet packet("a string");
		Assert::That(packet.getMessage(), Equals("a string"));
	}
};