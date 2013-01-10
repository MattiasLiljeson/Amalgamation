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
	ShipModule();
	~ShipModule();
};