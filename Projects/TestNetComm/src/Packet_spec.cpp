#include <igloo/igloo_alt.h>
using namespace igloo;

#include <Packet.h>
#include <AglVector3.h>
#include <AglQuaternion.h>

static const int TEST_PACKET_TYPE = 100;

Describe(a_packet)
{
	It(can_have_a_sender_id_defined_in_setSenderId_method)
	{
		Packet packet(TEST_PACKET_TYPE);
		packet.setSenderId(77);

		Assert::That(packet.getSenderId(), Equals(77));
	}

	It(can_contain_int_data)
	{
		Packet packet(TEST_PACKET_TYPE);

		int i_src = 1;
		packet << i_src;
		int i_dst;
		packet >> i_dst;

		Assert::That(i_dst, Equals(i_src));
	}

	It(can_contain_float_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		float f_src = 13.37f;
		packet << f_src;
		float f_dst;
		packet >> f_dst;

		Assert::That(f_dst, Equals(f_src));
	}

	It(can_contain_bool_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		bool b_src = true;
		packet << b_src;
		bool b_dst;
		packet >> b_dst;

		Assert::That(b_dst, Equals(b_src));
	}

	It(can_contain_char_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		char c_src = 100;
		packet << c_src;
		char c_dst;
		packet >> c_dst;

		Assert::That(c_dst, Equals(c_src));
	}

	It(can_contain_short_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		short s_src = 10000;
		packet << s_src;
		short s_dst;
		packet >> s_dst;

		Assert::That(s_dst, Equals(s_src));
	}

	It(can_contain_double_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		double d_src = 1337.1435000000377;
		packet << d_src;
		double d_dst;
		packet >> d_dst;

		Assert::That(d_dst, Equals(d_src));
	}

	It(can_contain_vec3_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		AglVector3 vec_src(1.0f, 2.0f, 3.0f);
		packet << vec_src;
		AglVector3 vec_dst;
		packet >> vec_dst;
		
		Assert::That(vec_dst.x, Equals(vec_src.x));
		Assert::That(vec_dst.y, Equals(vec_src.y));
		Assert::That(vec_dst.z, Equals(vec_src.z));
	}

	It(can_contain_quaternion_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		AglQuaternion quaternion_src(1.0f, 2.0f, 3.0f, 4.5f);
		packet << quaternion_src;
		AglQuaternion quaternion_dst;
		packet >> quaternion_dst;
		
		Assert::That(quaternion_dst.u.x, Equals(quaternion_src.u.x));
		Assert::That(quaternion_dst.u.y, Equals(quaternion_src.u.y));
		Assert::That(quaternion_dst.u.z, Equals(quaternion_src.u.z));
		Assert::That(quaternion_dst.v, Equals(quaternion_src.v));
	}

	It(can_contain_multiple_int_data)
	{
		Packet packet(TEST_PACKET_TYPE);

		int i_src[] = { 1, 2, 3 };
		packet << i_src[0] << i_src[1] << i_src[2];
		int i_dst[3];
		packet >> i_dst[0] >> i_dst[1] >> i_dst[2];

		for (int i = 0; i <3; i++)
			Assert::That(i_dst[i], Equals(i_src[i]));
	}

	It(can_contain_multiple_types_of_data)
	{
		Packet packet(TEST_PACKET_TYPE);
		int i_src = 42;
		float f_src = 13.37f;
		packet << i_src << f_src;
		int i_dst;
		float f_dst;
		packet >> i_dst >> f_dst;

		Assert::That(i_dst, Equals(i_src));
		Assert::That(f_dst, Equals(f_src));
	}

	It(can_be_cleared)
	{
		Packet packet(TEST_PACKET_TYPE);
		packet << 10 << 10.0f;

		packet.clear();
		Assert::That(packet.isEmpty(), Equals(true));
	}

	It(can_return_data_size)
	{
		Packet packet(TEST_PACKET_TYPE);

		int i_src[] = { 1, 2, 3 };
		packet << i_src[0] << i_src[1] << i_src[2];
		
		Assert::That(packet.getDataSize(), Equals(sizeof(i_src) + Packet::HEADER_SIZE));
	}

	It(can_be_empty)
	{
		Packet packet(TEST_PACKET_TYPE);
		
		Assert::That(packet.isEmpty(), IsTrue());
	}

	It(throws_an_exception_if_trying_to_stream_out_an_empty_packet)
	{
		Packet packet(TEST_PACKET_TYPE);
		int i_dst;
		AssertThrows(std::out_of_range, (packet >> i_dst));
	}

	It(can_set_byte_data)
	{
		char* byteData = new char[4 + Packet::HEADER_SIZE];

		// Header
		byteData[0] = 4;
		byteData[1] = 0;
		byteData[2] = 0;
		byteData[3] = 0;
		byteData[4] = 0;
		byteData[5] = 0;

		// An int 
		byteData[6] = 42;
		byteData[7] = 0;
		byteData[8] = 0;
		byteData[9] = 0;

		Packet packet(TEST_PACKET_TYPE);
		packet.setDataTest(byteData, 4 + Packet::HEADER_SIZE);

		int i_dst;
		packet >> i_dst;
		Assert::That(i_dst, Equals(42));

		delete[] byteData;
	}

	It(should_be_possible_to_copy_byte_data_from_a_packet_to_another)
	{
		int i_src = 5;
		float f_src = 6.9f;
		Packet packet_src(TEST_PACKET_TYPE);
		packet_src << i_src << f_src;

		unsigned int data_size = packet_src.getDataSize();
		char* data_src = packet_src.getDataPtr();

		Packet packet_dst(TEST_PACKET_TYPE);
		packet_dst.setDataTest(data_src, data_size);

		int i_dst;
		float f_dst;
		packet_dst >> i_dst >> f_dst;

		Assert::That(i_dst, Equals(i_src));
		Assert::That(f_dst, Equals(f_src));
	}

};