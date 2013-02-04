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
#include "EntityType.h"

class NetworkSynced: public Component
{
public:
	///-----------------------------------------------------------------------------------
	/// Sets the networkIdentity and owner to -1 (unspecified).
	/// \return 
	///-----------------------------------------------------------------------------------
	NetworkSynced()
		: Component( ComponentType::NetworkSynced )
	{
		m_networkIdentity = -1;
		m_owner = -1;
		m_networkType = EntityType::NON_EXISTENT;
	}

	///-----------------------------------------------------------------------------------
	/// Sets the private networkIdentity to argument's value.
	/// Sets the private owner to argument's value.
	/// \param p_networkIdentity
	/// \return 
	///-----------------------------------------------------------------------------------
	NetworkSynced( int p_networkIdentity, int p_owner )
		: Component( ComponentType::NetworkSynced )
	{
		m_networkIdentity = p_networkIdentity;
		m_owner = p_owner;
		m_networkType = EntityType::NON_EXISTENT;
	}

	NetworkSynced( int p_networkIdentity, int p_owner,
		EntityType::EntityEnums p_networkType )
		: Component( ComponentType::NetworkSynced )
	{
		m_networkIdentity = p_networkIdentity;
		m_owner = p_owner;
		m_networkType = p_networkType;
	}

	~NetworkSynced()
	{
	}

	///-----------------------------------------------------------------------------------
	/// Get the component's networkIdentity.
	/// \return int
	///-----------------------------------------------------------------------------------
	int getNetworkIdentity() const
	{
		return m_networkIdentity;
	}

	///-----------------------------------------------------------------------------------
	/// Get the component's owner.
	/// \return int
	///-----------------------------------------------------------------------------------
	int getNetworkOwner() const
	{
		return m_owner;
	}

	///-----------------------------------------------------------------------------------
	/// Get the network type.
	/// \return NetworkType
	///-----------------------------------------------------------------------------------
	EntityType::EntityEnums getNetworkType() const
	{
		return m_networkType;
	}

	///-----------------------------------------------------------------------------------
	/// Set the private networkIdentity.
	/// \param p_networkIdentity
	/// \return void
	///-----------------------------------------------------------------------------------
	void setNetworkIdentity( int p_networkIdentity )
	{
		m_networkIdentity = p_networkIdentity;
	}

	///-----------------------------------------------------------------------------------
	/// Set the private owner.
	/// \param p_owner
	/// \return void
	///-----------------------------------------------------------------------------------
	void setNetworkOwner( int p_owner )
	{
		m_owner = p_owner;
	}

	void setNetworkType( EntityType::EntityEnums p_networkType )
	{
		m_networkType = p_networkType;
	}

private:
	int m_networkIdentity;
	int m_owner;
	EntityType::EntityEnums m_networkType;
};