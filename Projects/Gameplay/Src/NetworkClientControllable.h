// =======================================================================================
//                                      NetworkClientControllable
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Mapped to a network client so that the client can control itself through
/// the network.
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
	///-----------------------------------------------------------------------------------
	/// Sets private clientIdentity to -1 (not specified).
	/// \returns 
	///-----------------------------------------------------------------------------------
	NetworkClientControllable()
	{
		m_clientIdentity = -1;
	}

	///-----------------------------------------------------------------------------------
	/// Sets private clientIdentity to argument's value.
	/// \param p_clientIdentity
	/// \returns 
	///-----------------------------------------------------------------------------------
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