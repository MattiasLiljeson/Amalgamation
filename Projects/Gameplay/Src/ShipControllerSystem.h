#pragma once

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

#include <EntitySystem.h>
#include <AglQuaternion.h>
#include "InputBackendSystem.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "ShipController.h"

class ShipControllerSystem : public EntitySystem
{
public:
	ShipControllerSystem(InputBackendSystem* p_inputBackend,
						 PhysicsSystem* p_physicsSystem);
	~ShipControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	InputBackendSystem* m_inputBackend;
	PhysicsSystem* m_physics;

	Control* m_horizontalPositive;
	Control* m_horizontalNegative;
	Control* m_verticalPositive;
	Control* m_verticalNegative;
	//
	Control* m_rollRight;
	Control* m_rollLeft;
	Control* m_thrust;
	Control* m_strafeHorizontalPositive;
	Control* m_strafeHorizontalNegative;
	Control* m_strafeVerticalPositive;
	Control* m_strafeVerticalNegative;
};