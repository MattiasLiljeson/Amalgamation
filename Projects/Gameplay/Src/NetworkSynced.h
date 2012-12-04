// =======================================================================================
//                                      NetworkSynced
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	NetworkSynced Component.
///        
/// # NetworkSynced
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <Component.h>

class NetworkSynced: public Component
{
public:
	///-----------------------------------------------------------------------------------
	/// Sets the networkIdentity to -1 (unspecified).
	/// \returns 
	///-----------------------------------------------------------------------------------
	NetworkSynced()
	{
		m_networkIdentity = -1;
	}

	///-----------------------------------------------------------------------------------
	/// Sets the private networkIdentity to argument's value.
	/// \param p_networkIdentity
	/// \returns 
	///-----------------------------------------------------------------------------------
	NetworkSynced( int p_networkIdentity )
	{
		m_networkIdentity = p_networkIdentity;
	}

	~NetworkSynced()
	{
	}

	///-----------------------------------------------------------------------------------
	/// Get the component's networkIdentity.
	/// \returns const int&
	///-----------------------------------------------------------------------------------
	const int& getNetworkIdentity() const
	{
		return m_networkIdentity;
	}

	///-----------------------------------------------------------------------------------
	/// Set the private networkIdentity.
	/// \param p_networkIdentity
	/// \returns void
	///-----------------------------------------------------------------------------------
	void setNetworkIdentity( int p_networkIdentity )
	{
		m_networkIdentity = p_networkIdentity;
	}

private:
	int m_networkIdentity;

};