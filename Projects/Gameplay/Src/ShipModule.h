#pragma once

#include <Component.h>

// =======================================================================================
//	Module
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Describes a module that can be attached to a ship
///        
/// # PlayerScore
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class ShipModule: public Component
{
public:
	int m_parentEntity;
	bool m_active;
	float m_value; ///< Value of the module. Generates score.
	float m_health; ///< Health of the module. A module that loses all health is removed

	ShipModule();
	~ShipModule();
};