#pragma once
#include "Packetizer.h"
#include <vector>
using namespace std;

// =======================================================================================
//                                      EntityCreationPacket
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the packing and unpacking of entity creations.
///        
/// # EntityCreationPacket
/// Detailed description.....
/// Created on: 14-1-2013 
///---------------------------------------------------------------------------------------

class EntityCreationPacket : public Packetizer
{
public:
	EntityCreationPacket();
	~EntityCreationPacket();

	virtual Packet pack();

	virtual void unpack( Packet& p_packet );

public:
	char			entityType;
	bool			isLevelProp;
	int				owner;
	int				playerID;
	int				networkIdentity;
	int				meshInfo;
	int				miscData; ///< extra data if needed
	AglVector3		translation;
	AglVector3		scale;
	AglQuaternion	rotation;

	// Additional misc data. This vector has no limit in itself, but should not contain
	// to much data, as it could get larger than what's allowed in a packet.
	vector<char> additionalMisc;
};