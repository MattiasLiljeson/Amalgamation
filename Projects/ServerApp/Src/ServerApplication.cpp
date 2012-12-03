#include "ServerApplication.h"

ServerApplication::ServerApplication()
{
	m_running = false; // set running to true if initialization is ok!

	m_ioService = NULL;
	m_tcpListenerProcess = NULL;

	initNetwork();
}

ServerApplication::~ServerApplication()
{
	if( m_ioService )
		delete m_ioService;
}

void ServerApplication::run()
{
	
}

void ServerApplication::initNetwork()
{
	m_ioService = new boost::asio::io_service();

	m_tcpListenerProcess = new TcpListenerProcess( NULL, 1337, m_ioService );
}

void ServerApplication::initSystems()
{

}
