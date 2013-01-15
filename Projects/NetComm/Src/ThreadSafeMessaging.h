#pragma once

#include <queue>


#include "ProcessMessage.h"
using namespace std;

namespace boost
{
	class mutex;
};

class ProcessMessage;

///
/// Abstract ThreadSafeMessaging provides safe communication between threads/processes
/// using messages.
///
class ThreadSafeMessaging
{
public:
	ThreadSafeMessaging();
	virtual ~ThreadSafeMessaging();

	///-----------------------------------------------------------------------------------
	/// Put a message on ThreadSafeMessaging's own message queue.
	/// \param ProcessMessage * p_message
	/// \return void
	///-----------------------------------------------------------------------------------
	void putMessage( ProcessMessage* p_message );

	///-----------------------------------------------------------------------------------
	/// Put a queue of messages on the process' message queue.
	/// \param p_messages
	/// \return void
	///-----------------------------------------------------------------------------------
	void putMessages( queue<ProcessMessage*> p_messages );

	virtual void processMessages() = 0;

protected:
	///-----------------------------------------------------------------------------------
	/// Checking out the message queue means you take ownership over it.
	/// \return queue< ProcessMessage* >
	///-----------------------------------------------------------------------------------
	queue< ProcessMessage* > checkoutMessageQueue();

private:
	queue< ProcessMessage* > m_messageQueue; /**< Stores all messages that are "put"
											on the ThreadSafeMessaging instance. Every
											access to the queue must be thread safe. */

	boost::mutex* m_guard; /**< A unique lock that is used when accessing the message
						  queue. */

};