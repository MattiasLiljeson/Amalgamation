#pragma once

#include <EntitySystem.h>

class ShipInputProcessingSystem;

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
	PlayerCameraControllerSystem(ShipInputProcessingSystem* p_shipInput);
	~PlayerCameraControllerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

protected:
private:

	ShipInputProcessingSystem* m_shipInput;	
};