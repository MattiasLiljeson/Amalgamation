#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                      StateManagementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Manages how entities and components are created and deleted during states.
///        
/// # StateManagementSystem
/// Detailed description.....
/// Created on: 14-3-2013 
///---------------------------------------------------------------------------------------

class StateManagementSystem : public EntitySystem
{
public:
	StateManagementSystem();
	
	virtual ~StateManagementSystem();

	virtual void process();

	virtual void initialize();

protected:
private:
};