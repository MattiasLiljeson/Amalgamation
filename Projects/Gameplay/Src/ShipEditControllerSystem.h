#pragma once

#include <EntitySystem.h>

class Control;
class TcpClient;
class InputBackendSystem;
class PhysicsSystem;
class ShipInputProcessingSystem;

// =======================================================================================
//                                   ShipEditControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System that handles the edit mode for the ship
///        
/// # ShipEditControllerSystem
/// Detailed description.....
/// Created on: 9-1-2013 
///---------------------------------------------------------------------------------------

class ShipEditControllerSystem : public EntitySystem
{
public:
	ShipEditControllerSystem(ShipInputProcessingSystem* p_shipInput,
					   PhysicsSystem* p_physicsSystem/*,
					   TcpClient* p_client */);
	~ShipEditControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

protected:
private:
	PhysicsSystem* m_physics;
	// TcpClient* m_client;

	ShipInputProcessingSystem* m_shipInput;	
};