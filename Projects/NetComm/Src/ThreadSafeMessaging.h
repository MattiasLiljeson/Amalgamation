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

	void putMessage( ProcessMessage* p_message );

protected:
	ProcessMessage* popMessage();

private:
	queue< ProcessMessage* > m_messageQueue;
	boost::mutex m_guard;

};