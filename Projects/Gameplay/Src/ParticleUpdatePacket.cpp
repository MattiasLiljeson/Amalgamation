#include "ParticleUpdatePacket.h"
#include "PacketType.h"

ParticleUpdatePacket::ParticleUpdatePacket()
{
	particleSystemIdx = -1;
	networkIdentity = -1;
	position = AglVector3();
	direction = AglVector3(1, 0, 0);
	speed = 1;
	spawnFrequency = 1;
}

ParticleUpdatePacket::~ParticleUpdatePacket()
{

}

Packet ParticleUpdatePacket::pack()
{
	Packet packet(static_cast<char>(PacketType::ParticleUpdate));	
	packet << particleSystemIdx
		<< networkIdentity
		<< position
		<< direction
		<< speed
		<< spawnFrequency;

	return packet;
}

void ParticleUpdatePacket::unpack( Packet p_packet )
{
	p_packet >> particleSystemIdx
		>> networkIdentity 
		>> position 
		>> direction
		>> speed
		>> spawnFrequency;
}
