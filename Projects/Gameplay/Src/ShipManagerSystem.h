#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                   ShipManagerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Manages all ships. Contains a list of these
///        
/// # ShipManagerSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class ShipManagerSystem : public EntitySystem
{
public:
	ShipManagerSystem();
	const vector<Entity*>& getShips() const;
	Entity* findShip(int p_ownerId);

};
