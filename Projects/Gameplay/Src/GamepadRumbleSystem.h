#pragma once
#include <EntitySystem.h>

class XInputFetcher;
class InputBackendSystem;
class NetworkSynced;

// =======================================================================================
//                                      InputSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # InputSystem
/// Detailed description.....
/// Created on: 4-12-2012 
///---------------------------------------------------------------------------------------

class GamepadRumbleSystem : public EntitySystem
{
public:
	GamepadRumbleSystem(InputBackendSystem* p_input);
	~GamepadRumbleSystem();

	void processEntities( const vector<Entity*>& p_entities );
private:
	void sendRumblePacketToServer(NetworkSynced* p_syncedInfo, 
		float p_power1, float p_power2);

	InputBackendSystem* m_input;
};

