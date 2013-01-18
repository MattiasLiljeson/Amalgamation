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
	~ShipManagerSystem();

	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

	vector<Entity*> getShips();
	Entity* findShip(int p_ownerId);
private:
	vector<Entity*> m_ships;

};
