#pragma once

#include "Packetizer.h"

// =======================================================================================
//                                      AttachModule
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	
///        
/// # AttachModule
/// Detailed description.....
/// Created on: 18-2-2013 
///---------------------------------------------------------------------------------------

class ModuleStateChangePacket : public Packetizer
{
public:
	ModuleStateChangePacket();

	Packet pack();

	void unpack( Packet& p_packet );
public:
	int affectedModule;
	int previousParent;
	int currentParrent;
};