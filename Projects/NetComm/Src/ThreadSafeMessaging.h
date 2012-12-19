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

	virtual void processMessages() = 0;

protected:
	///-----------------------------------------------------------------------------------
	/// Pop oldest message from the message queue. Take over ownership of the message.
	/// \return ProcessMessage*
	///-----------------------------------------------------------------------------------
	ProcessMessage* popMessage();

	///-----------------------------------------------------------------------------------
	/// Get the amount of messages currently stored in the message queue.
	/// \return unsigned int
	///-----------------------------------------------------------------------------------
	unsigned int getMessageCount();

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