#pragma once

#include <EntitySystem.h>

class TcpClient;
class ShipInputProcessingSystem;
class NetworkSynced;
struct AglVector3;

// =======================================================================================
//                              PlayerCameraControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System that handles camera behaviour based on input
///        
/// #PlayerCameraControllerSystem
/// Detailed description.....
/// Created on: 9-1-2013 
///---------------------------------------------------------------------------------------

class PlayerCameraControllerSystem : public EntitySystem
{
public:
	PlayerCameraControllerSystem(ShipInputProcessingSystem* p_shipInput,
		TcpClient* p_client);
	~PlayerCameraControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

protected:
private:

	void sendCameraControllerPacketToServer(NetworkSynced* p_syncedInfo, AglVector3& p_movement,
		int p_state);

	ShipInputProcessingSystem* m_shipInput;	
	TcpClient* m_client;
};