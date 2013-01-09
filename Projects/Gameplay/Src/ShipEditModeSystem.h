#pragma once

#include <EntitySystem.h>

class Control;
class TcpClient;
class InputBackendSystem;
class PhysicsSystem;

// =======================================================================================
//                                      ShipEditModeSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System that handles the edit mode for the ship
///        
/// # ShipEditModeSystem
/// Detailed description.....
/// Created on: 9-1-2013 
///---------------------------------------------------------------------------------------

class ShipEditControllerSystem : public EntitySystem
{
public:
	ShipEditControllerSystem(InputBackendSystem* p_inputBackend,
					   PhysicsSystem* p_physicsSystem/*,
					   TcpClient* p_client */);
	~ShipEditControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

protected:
private:
};