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
	NetworkSynced()
	{
		networkIdentity = -1;
	}

	NetworkSynced( int p_networkIdentity )
	{
		networkIdentity = p_networkIdentity;
	}

	~NetworkSynced()
	{
	}

private:
	int networkIdentity;

};