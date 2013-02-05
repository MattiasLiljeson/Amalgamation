#pragma once

#include <Rocket/Core/String.h>
// =======================================================================================
//                                      EventManager
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # EventManager
/// Detailed description.....
/// Created on: 5-2-2013 
///---------------------------------------------------------------------------------------
class EventHandler;

namespace Rocket
{
	namespace Core
	{
		class Event;
	}
}

class EventManager
{
public:
	EventManager(){};
	virtual ~EventManager(){};
	virtual void ProcessEvent(Rocket::Core::Event& p_event, const Rocket::Core::String& p_value) = 0;
	virtual void RegisterEventHandler(EventHandler* p_eventHandler) = 0;
};