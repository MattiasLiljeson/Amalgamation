// =======================================================================================
//                                      PhysicsBody
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # PhysicsBody
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------
#pragma once
#include <Component.h>
#include <AglVector3.h>

class PhysicsBody : public Component
{
public:
	PhysicsBody()
	{
		m_id = -1;
	}
	int m_id;
};