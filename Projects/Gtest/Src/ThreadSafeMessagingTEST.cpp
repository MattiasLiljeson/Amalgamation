#include <gtest/gtest.h>
#include <string>

#include "ThreadSafeMessaging.h"

using namespace std;

class TestMessenger : public ThreadSafeMessaging
{
public:
	TestMessenger()
	{
		m_messaging = NULL;
	}

	TestMessenger( ThreadSafeMessaging* p_messaging )
	{
		m_messaging = p_messaging;
	}

	~TestMessenger()
	{
	}

	void sendMessage( string p_message )
	{
		m_messaging->putMessage( new ProcessMessage(
			MessageType::TEXT, m_messaging, p_message ) );
	}

	string getMessage()
	{
		ProcessMessage* message = popMessage();

		string outMessage = message->message;

		delete message;

		return outMessage;
	}

private:
	ThreadSafeMessaging* m_messaging;


};

TEST(ThreadSafeMessaging, PutMessage)
{
	ThreadSafeMessaging* A = new TestMessenger();

	TestMessenger* B = new TestMessenger(A);
	string hej = "hej";
	B->sendMessage( hej );

	EXPECT_STREQ( hej.c_str(), static_cast<TestMessenger*>(A)->getMessage().c_str() );
}