#pragma once

#include "ProcessMessage.h"

class ProcessMessageSocketDisconnected: public ProcessMessage
{
public:
	ProcessMessageSocketDisconnected();

	ProcessMessageSocketDisconnected( ThreadSafeMessaging* p_sender,
		int p_processId );

	~ProcessMessageSocketDisconnected();

public:
	int processId;

};