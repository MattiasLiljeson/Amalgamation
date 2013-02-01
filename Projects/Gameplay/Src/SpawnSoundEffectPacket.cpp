#include "SpawnSoundEffectPacket.h"

map<int, string> SpawnSoundEffectPacket::soundEffectMapper;


SpawnSoundEffectPacket::SpawnSoundEffectPacket()
{
	soundIdentifier = -1;
	attachedToNetsyncEntity = -1;
	positional = false;

	SpawnSoundEffectPacket::soundEffectMapper[(int)SoundIdentities::SpaceShipEngineIdle_one] =
		"Spaceship_Engine_Idle_-_Spaceship_Onboard_Cruise_Rumble_Drone_Subtle_Slow_Swells.wav";
	SpawnSoundEffectPacket::soundEffectMapper[(int)SoundIdentities::LaserGun] =
		"spaceship_laser.wav";
	SpawnSoundEffectPacket::soundEffectMapper[(int)SoundIdentities::MissileStartAndFlight] =
		"Missile_Start_and_Flight.wav";
	SpawnSoundEffectPacket::soundEffectMapper[(int)SoundIdentities::Explosion] =
		"bomb-03.wav";
	SpawnSoundEffectPacket::soundEffectMapper[(int)SoundIdentities::MineUnload] =
		"Mine_Unload.wav";
	SpawnSoundEffectPacket::soundEffectMapper[(int)SoundIdentities::ShieldDeflect] =
		"Shield_Deflect.wav";
}

Packet SpawnSoundEffectPacket::pack()
{
	Packet packet(static_cast<char>(PacketType::SpawnSoundEffect));
	packet << soundIdentifier;
	packet << attachedToNetsyncEntity;
	packet << positional;
	packet << position;
	return packet;
}

void SpawnSoundEffectPacket::unpack( Packet p_packet )
{
	p_packet >> soundIdentifier;
	p_packet >> attachedToNetsyncEntity;
	p_packet >> positional;
	p_packet >> position;
}