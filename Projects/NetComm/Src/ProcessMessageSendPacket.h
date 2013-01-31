#pragma once

#include "Packet.h"
#include "ProcessMessage.h"

class ProcessMessageSendPacket: public ProcessMessage
{
public:
	ProcessMessageSendPacket( ThreadSafeMessaging* p_sender, Packet p_packet );

	~ProcessMessageSendPacket();

public:
	Packet packet;

};