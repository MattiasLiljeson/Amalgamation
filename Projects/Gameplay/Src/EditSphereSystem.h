#pragma once

#include <EntitySystem.h>
#include <AglVector3.h>

class Control;
class TcpClient;
class InputBackendSystem;
class PhysicsSystem;
class ShipInputProcessingSystem;
class SlotInputControllerSystem;

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

class EditSphereSystem : public EntitySystem
{
public:
	EditSphereSystem();
	~EditSphereSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );
	void setSphere(AglVector3 p_position, float p_radius);

protected:
private:
	Entity* m_editSphere;
};