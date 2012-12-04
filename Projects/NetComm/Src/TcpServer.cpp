#include "TcpServer.h"

TcpServer::TcpServer()
{
	m_isListening = false;
}

TcpServer::~TcpServer()
{

}

void TcpServer::startListening( int p_port )
{
	m_isListening = true;
}

void TcpServer::stopListening()
{
	m_isListening = false;
}

bool TcpServer::isListening()
{
	return m_isListening;
}

bool TcpServer::hasNewConnections()
{
	return false;
}