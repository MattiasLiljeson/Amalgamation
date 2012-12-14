// =======================================================================================
//	Identity
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # Identity
/// Detailed description...
/// Created on: 14-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <Component.h>

class Identity: public Component
{
public:
	Identity()
	{
		m_identity = -1;
	}

	Identity( int p_identity )
	{
		m_identity = p_identity;
	}

	~Identity()
	{
	}

	int getIdentity()
	{
		return m_identity;
	}

	void setIdentity( int p_identity )
	{
		m_identity = p_identity;
	}

private:
	int m_identity;

};