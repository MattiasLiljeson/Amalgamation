// =======================================================================================
//                                      NetworkClientControllable
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief description...
///        
/// # NetworkClientControllable
/// Detailed description...
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <Component.h>

class NetworkClientControllable: public Component
{
public:
	NetworkClientControllable()
	{
		m_clientIdentity = -1;
	}

	NetworkClientControllable( int p_clientIdentity )
	{
		m_clientIdentity = p_clientIdentity;
	}

	~NetworkClientControllable()
	{
	}

private:
	int m_clientIdentity;

};