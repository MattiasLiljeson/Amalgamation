#pragma once

// =======================================================================================
//                                      ShipEditController
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Ship edit controller component. Defines editing controller properties of 
/// the ship.
///        
/// # ShipController
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

#include <Component.h>
#include <DebugUtil.h>
#include <ToString.h>

class ShipEditController : public Component
{
public:
	ShipEditController()
	{
		m_nada=0.0f;
	}


	~ShipEditController() {}


private:
	float m_nada;
};