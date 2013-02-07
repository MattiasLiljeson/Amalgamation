#pragma once

#include <EntitySystem.h>
#include <EventHandler.h>
// =======================================================================================
//                                      GameOptionsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles managing and changing of game settings.
///        
/// # GameOptionsSystem
/// This class can be used to load and save game settings.
/// Created on: 5-2-2013 
/// @Albc
///---------------------------------------------------------------------------------------

class GameOptionsSystem : public EntitySystem, public EventHandler
{
public:
	GameOptionsSystem();
	virtual ~GameOptionsSystem();


	virtual void processEvent( Rocket::Core::Event& p_event, const Rocket::Core::String& p_value );

};