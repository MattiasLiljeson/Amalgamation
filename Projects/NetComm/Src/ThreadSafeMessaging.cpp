#include "ThreadSafeMessaging.h"

#include <boost/thread/mutex.hpp>

ThreadSafeMessaging::ThreadSafeMessaging()
{
	m_guard = new boost::mutex();
}

ThreadSafeMessaging::~ThreadSafeMessaging()
{
	m_guard->lock();

	while( m_messageQueue.size() > 0 )
	{
		if( m_messageQueue.front() )
		{
			delete m_messageQueue.front();
			m_messageQueue.pop();
		}
	}

	m_guard->unlock();
	delete m_guard;
}

void ThreadSafeMessaging::putMessage( ProcessMessage* p_message )
{
	if( p_message )
	{
		m_guard->lock();

		m_messageQueue.push( p_message );

		m_guard->unlock();
	}
}

void ThreadSafeMessaging::putMessages( queue<ProcessMessage*> p_messages )
{
	if( p_messages.size() > 0 )
	{
		m_guard->lock();

		while( p_messages.size() > 0 )
		{
			m_messageQueue.push( p_messages.front() );
			p_messages.pop();
		}

		m_guard->unlock();
	}
}

queue< ProcessMessage* > ThreadSafeMessaging::checkoutMessageQueue()
{
	queue< ProcessMessage* > messages;

	m_guard->lock();

	while( m_messageQueue.size() > 0 )
	{
		messages.push( m_messageQueue.front() );
		m_messageQueue.pop();
	}

	m_guard->unlock();

	return messages;
}
