#pragma once

#include <EntitySystem.h>
#include <AglVector3.h>

class Control;
class TcpClient;
class PhysicsSystem;
class ShipInputProcessingSystem;
class NetworkSynced;
class Transform;

// =======================================================================================
//                                ShipControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for controlling a ship
///        
/// # ShipControllerSystem
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

class ShipFlyControllerSystem : public EntitySystem
{
public:
	ShipFlyControllerSystem(ShipInputProcessingSystem* p_inputBackend,
						 PhysicsSystem* p_physicsSystem,
						 TcpClient* p_client );
	~ShipFlyControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	//Anton - 9/1-13
	float	getSpeedBoost(Entity* p_entity, float p_baseThrust);
	void	sendThrustPacketToServer(NetworkSynced* p_syncedInfo, AglVector3 p_thrust,
		AglVector3 p_angularVec);
	void	sendTransformPacketToServer(NetworkSynced* p_syncedInfo, Transform* p_transform);
private:
	PhysicsSystem* m_physics;
	TcpClient* m_client;

	ShipInputProcessingSystem* m_shipInput;

	AglVector3 m_thrustVec;
	AglVector3 m_angularVec;
};