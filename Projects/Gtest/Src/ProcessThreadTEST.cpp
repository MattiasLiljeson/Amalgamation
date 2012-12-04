#include <gtest/gtest.h>
#include <queue>

#include "ProcessThread.h"

using namespace std;

class TestProcessThread : public ProcessThread
{
public:
	TestProcessThread()
	{
		m_running = true;
	}

	virtual void body() 
	{
		while( m_running )
		{
			if( getMessageCount() > 0 )
			{
				queue< ProcessMessage* > messages;
				messages = checkoutMessageQueue();

				while( messages.size() > 0 )
				{
					ProcessMessage* msg = messages.front();
					messages.pop();

					if( msg->type = MessageType::TERMINATE )
						m_running = false;

					delete msg;
				}
			}
		}
	}

private:
	bool m_running;

};

TEST(TestProcessThread, FirstRun)
{
	TestProcessThread* proc;
	proc = new TestProcessThread();

	proc->start();
	proc->stop();

	delete proc;
}