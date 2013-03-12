#pragma once

#include "Packet.h"
#include "ProcessMessage.h"

class ProcessMessageReceivePacket: public ProcessMessage
{
public:
	ProcessMessageReceivePacket( ThreadSafeMessaging* p_sender, Packet p_packet );

	~ProcessMessageReceivePacket();

public:
	Packet packet;

};