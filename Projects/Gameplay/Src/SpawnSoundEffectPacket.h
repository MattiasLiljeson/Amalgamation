#pragma once

#include "Packetizer.h"
#include <AglVector3.h>
#include <map>
using namespace std;

// =======================================================================================
//	SpawnSoundEffectPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # SpawnSoundEffectPacket
/// Detailed description...
/// Created on: 23-1-2013 
///---------------------------------------------------------------------------------------

class SpawnSoundEffectPacket: public Packetizer
{
public:
	SpawnSoundEffectPacket();
	Packet pack();
	void unpack( Packet p_packet );

public:
	int soundIdentifier;
	int attachedToNetsyncEntity; // Used if not -1 (which is default).
	bool positional;
	AglVector3 position; // Only used if (positional) is true.

public:
	/************************************************************************/
	/* Code below helps with identifying a sound file over the network.		*/
	/************************************************************************/

	enum SoundIdentities {
		SpaceShipEngineIdle_one,
		LaserGun,
		MissileStartAndFlight,
		Explosion,
		MineUnload,
		MineBlip,
		ShieldDeflect,
	};
	// NOTE: (Johan) This could be made static and initialized only once, but I don't
	// know how to do it. It works fine as it is though.
	// Also it's initialized in the constructor!
	static map<int, string> soundEffectMapper;
	
};