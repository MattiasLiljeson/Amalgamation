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
#include "NetworkType.h"

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
		m_networkType.type = NetworkType::NON_EXISTENT;
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
		m_networkType.type = NetworkType::NON_EXISTENT;
	}

	NetworkSynced( int p_networkIdentity, int p_owner, NetworkType::Network_T p_networkType )
	{
		m_networkIdentity = p_networkIdentity;
		m_owner = p_owner;
		m_networkType.type = p_networkType;
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
	/// Get the network type.
	/// \returns NetworkType
	///-----------------------------------------------------------------------------------
	NetworkType::Network_T getNetworkType() const
	{
		return m_networkType.type;
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

	void setNetworkType( NetworkType::Network_T p_networkType )
	{
		m_networkType.type = p_networkType;
	}

private:
	int m_networkIdentity;
	int m_owner;
	NetworkType m_networkType;
};