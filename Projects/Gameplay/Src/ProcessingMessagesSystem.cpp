#include "ProcessingMessagesSystem.h"
#include <ThreadSafeMessaging.h>


ProcessingMessagesSystem::ProcessingMessagesSystem(
	ThreadSafeMessaging* p_messageProcesser )
	: EntitySystem( SystemType::ProcessingMessagesSystem )
{
	m_messageProcesser = p_messageProcesser;
}

ProcessingMessagesSystem::~ProcessingMessagesSystem()
{

}

void ProcessingMessagesSystem::process()
{
	m_messageProcesser->processMessages();
}

void ProcessingMessagesSystem::initialize()
{
}
