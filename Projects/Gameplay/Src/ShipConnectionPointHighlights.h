#pragma once

#include <Component.h>
#include <ComponentFactory.h>

// =======================================================================================
//                               ShipConnectionPointHighlights
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ShipConnectionPointHighlights
/// Detailed description.....
/// Created on: 7-2-2013 
///---------------------------------------------------------------------------------------

class ShipConnectionPointHighlights : public Component
{
public:
	ShipConnectionPointHighlights();
	~ShipConnectionPointHighlights(){}
public:
	bool slotStatus[4];
	int current;
};