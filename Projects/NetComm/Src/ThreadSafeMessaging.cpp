#include "ThreadSafeMessaging.h"


ThreadSafeMessaging::ThreadSafeMessaging()
{
}

ThreadSafeMessaging::~ThreadSafeMessaging()
{
	m_guard.lock();

	while( m_messageQueue.size() > 0 )
	{
		if( m_messageQueue.front() )
		{
			delete m_messageQueue.front();
			m_messageQueue.pop();
		}
	}

	m_guard.unlock();
}

void ThreadSafeMessaging::putMessage( ProcessMessage* p_message )
{
	if( p_message )
	{
		m_guard.lock();

		m_messageQueue.push( p_message );

		m_guard.unlock();
	}
}

ProcessMessage* ThreadSafeMessaging::popMessage()
{
	ProcessMessage* message = NULL;

	m_guard.lock();

	message = m_messageQueue.front();
	m_messageQueue.pop();

	m_guard.unlock();


	return message;
}

unsigned int ThreadSafeMessaging::getMessageCount()
{
	unsigned int messageCount = 0;

	m_guard.lock();

	messageCount = m_messageQueue.size();

	m_guard.unlock();

	return messageCount;
}

queue< ProcessMessage* > ThreadSafeMessaging::checkoutMessageQueue()
{
	queue< ProcessMessage* > messages;

	m_guard.lock();

	while( m_messageQueue.size() > 0 )
	{
		messages.push( m_messageQueue.front() );
		m_messageQueue.pop();
	}

	m_guard.unlock();

	return messages;
}
