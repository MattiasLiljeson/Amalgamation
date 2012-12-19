#include <igloo/igloo_alt.h>
using namespace igloo;

#include <Packet.h>
#include <PacketTranslator.h>

class ExamplePacket: public PacketTranslator
{
public:
	ExamplePacket(int p_number, float p_value)
	{
		m_number = p_number;
		m_value = p_value;
	}

	virtual char* getBytes() 
	{
	}

	virtual void setBytes( char* p_byteData, unsigned int p_bytesSize ) 
	{
	}

private:
	int m_number;
	float m_value;

};

Describe(a_packet_translator)
{
	It(can_translate_a_class_into_byte_data)
	{
		PacketTranslator* packetTranslator;
		int number = 5;
		float value = 7.3f;
		packetTranslator = new ExamplePacket(number, value);

		Packet packet_dst;
		packet_dst.setData(packetTranslator->getBytes(),
			packetTranslator->getBytesSize());


		int number_dst;
		float value_dst;
		packet_dst >> number_dst >> value_dst;
		Assert::That(number_dst, Equals(number));
		Assert::That(value_dst, Equals(value));
	}

};