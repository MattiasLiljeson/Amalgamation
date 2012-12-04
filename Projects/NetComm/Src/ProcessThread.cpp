#include "ProcessThread.h"


ProcessThread::ProcessThread()
{
	m_id = s_nextId;
	s_nextId += 1;

	m_thread = NULL;
}

ProcessThread::~ProcessThread()
{
	stop();
}

int ProcessThread::start()
{
	int result = 1;

	if( m_thread == NULL )
	{
		m_thread = new boost::thread(
			boost::bind(
			&ProcessThread::body, this ) );
		result = 0;
	}

	return result;
}

void ProcessThread::stop()
{
	if( m_thread )
	{
//		// Send "exit" message/event to the process.
//		putMessage( new ProcessMessageTerminate( this ) );

		m_thread->join();
		delete m_thread;
		m_thread = NULL;
	}
}

void ProcessThread::restart()
{
	stop();
	start();
}

const int ProcessThread::getId() const
{
	return m_id;
}

void ProcessThread::interrupt()
{
	if( m_thread )
	{
		m_thread->interrupt();
		delete m_thread;
		m_thread = NULL;
	}
}

int ProcessThread::s_nextId = 0;
