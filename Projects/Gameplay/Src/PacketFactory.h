// =======================================================================================
//	PacketFactory
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # PacketFactory
/// Detailed description...
/// Created on: 18-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include "Packet.h"
#include "PacketType.h"
#include "NetworkType.h"

class PacketFactory
{
public:
	PacketFactory();
	~PacketFactory();

	Packet CreateEntityCreatePacket(NetworkType p_type);
	Packet CreateEntityUpdatePacket(NetworkType p_type);

private:

};