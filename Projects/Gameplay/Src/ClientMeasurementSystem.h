#pragma once
#include <EntitySystem.h>
// =======================================================================================
// ClientMeasurementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ClientMeasurementSystem
/// Detailed description...
/// Created on: 6-2-2013 
///---------------------------------------------------------------------------------------
class ClientMeasurementSystem: public EntitySystem
{
public:
	ClientMeasurementSystem();
	~ClientMeasurementSystem();
	void initialize();

};