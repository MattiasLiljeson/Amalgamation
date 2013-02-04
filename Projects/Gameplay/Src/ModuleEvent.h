#pragma once
// =======================================================================================
// ModuleEvent
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief ModuleEvent interface.
///        
/// # ModuleEvent
/// Realize/implement the pure virtual function and you can use the event in ShipModule.
/// Created on: 1-2-2013 
///---------------------------------------------------------------------------------------
class ModuleEvent
{
public:
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};