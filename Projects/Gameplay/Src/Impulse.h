// =======================================================================================
//                                      Impulse
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Impulse
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <Component.h>
#include <AglVector3.h>

class Impulse : public Component
{
public:
	Impulse()
	{
	}

	Impulse( AglVector3 p_impulse, AglVector3 p_angularImpulse )
	{
		m_impulse = p_impulse;
		m_angularImpulse = p_angularImpulse;
	}
	AglVector3 getImpulse() const
	{
		return m_impulse;
	}
	AglVector3 getAngularImpulse() const
	{
		return m_angularImpulse;
	}

private:
	AglVector3 m_impulse;
	AglVector3 m_angularImpulse;

};