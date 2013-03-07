#pragma once
#include <Component.h>
#include <AglVector3.h>
// =======================================================================================
//                                      ThrustComponent
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the thrust applied to the object
///        
/// # ThrustComponent
/// Detailed description.....
/// Created on: 7-3-2013 
///---------------------------------------------------------------------------------------

class ThrustComponent : public Component
{
public:
	ThrustComponent();
	~ThrustComponent();
public:
	AglVector3 thrustVector;
	AglVector3 angularVector;
};