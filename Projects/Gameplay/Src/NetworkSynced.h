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
	/// Sets the networkIdentity and owner to -1 (unspecified).
	/// \returns 
	///-----------------------------------------------------------------------------------
	NetworkSynced()
	{
		m_networkIdentity = -1;
		m_owner = -1;
	}

	///-----------------------------------------------------------------------------------
	/// Sets the private networkIdentity to argument's value.
	/// Sets the private owner to argument's value.
	/// \param p_networkIdentity
	/// \returns 
	///-----------------------------------------------------------------------------------
	NetworkSynced( int p_networkIdentity, int p_owner )
	{
		m_networkIdentity = p_networkIdentity;
		m_owner = p_owner;
	}

	~NetworkSynced()
	{
	}

	///-----------------------------------------------------------------------------------
	/// Get the component's networkIdentity.
	/// \returns int
	///-----------------------------------------------------------------------------------
	int getNetworkIdentity() const
	{
		return m_networkIdentity;
	}

	///-----------------------------------------------------------------------------------
	/// Get the component's owner.
	/// \returns int
	///-----------------------------------------------------------------------------------
	int getNetworkOwner() const
	{
		return m_owner;
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

	///-----------------------------------------------------------------------------------
	/// Set the private owner.
	/// \param p_owner
	/// \returns void
	///-----------------------------------------------------------------------------------
	void setNetworkOwner( int p_owner )
	{
		m_owner = p_owner;
	}

private:
	int m_networkIdentity;
	int m_owner;
};