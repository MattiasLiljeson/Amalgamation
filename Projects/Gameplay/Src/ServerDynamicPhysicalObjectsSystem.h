#pragma once
#include <EntitySystem.h>
// =======================================================================================
// ServerDynamicPhysicalObjectsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Tracks entities with components: NetworkSynced, Transform, PhysicsBody.
///        
/// # ServerDynamicPhysicalObjectsSystem
/// Detailed description...
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------
class ServerDynamicPhysicalObjectsSystem: public EntitySystem
{
public:
	ServerDynamicPhysicalObjectsSystem();
	// Get the active entities!
};