#pragma once

// =======================================================================================
//                                      ShipControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for controlling a ship
///        
/// # ShipControllerSystem
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

#include "CameraInfo.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"
#include "Transform.h"
#include <EntitySystem.h>

class ShipControllerSystem
{
public:
	ShipControllerSystem(InputBackendSystem* p_inputBackend);
	~ShipControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

private:
	InputBackendSystem* m_inputBackend;

	Control* m_mouseXPositive;
	Control* m_mouseXNegative;
	Control* m_mouseYPositive;
	Control* m_mouseYNegative;


	float m_ticker;
};