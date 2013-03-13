#pragma once
#include <EntitySystem.h>
#include "GameSettingsInfo.h"

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
	GamepadRumbleSystem(InputBackendSystem* p_input, const GameSettingsInfo& p_settings);
	~GamepadRumbleSystem();

	void initialize();

	void processEntities( const vector<Entity*>& p_entities );
	void setRumbleEnabled( bool p_enabled ){ m_enabled = p_enabled; }
	void disable() {m_enabled=false;}
	void enable() {m_enabled=true;}
private:
	void sendRumblePacketToServer(NetworkSynced* p_syncedInfo, 
		float p_power1, float p_power2);

	InputBackendSystem* m_input;
	bool m_enabled;
};

