#pragma once

#include <queue>
#include <boost/thread.hpp>

using namespace std;

class ProcessMessage;

///
/// Abstract ThreadSafeMessaging provides safe communication between threads/processes
/// using messages.
///
class ThreadSafeMessaging
{
public:
	ThreadSafeMessaging();
	~ThreadSafeMessaging();

	///-----------------------------------------------------------------------------------
	/// Put a message on ThreadSafeMessaging's own message queue.
	/// \param ProcessMessage * p_message
	/// \returns void
	///-----------------------------------------------------------------------------------
	void putMessage( ProcessMessage* p_message );

protected:
	///-----------------------------------------------------------------------------------
	/// Pop oldest message from the message queue.
	/// \returns ProcessMessage*
	///-----------------------------------------------------------------------------------
	ProcessMessage* popMessage();

	///-----------------------------------------------------------------------------------
	/// Get the amount of messages currently stored in the message queue.
	/// \returns unsigned int
	///-----------------------------------------------------------------------------------
	unsigned int getMessageCount();

private:
	queue< ProcessMessage* > m_messageQueue; /**< Stores all messages that are "put"
											on the ThreadSafeMessaging instance. Every
											access to the queue must be thread safe. */

	boost::mutex m_guard; /**< A unique lock that is used when accessing the message
						  queue. */

};