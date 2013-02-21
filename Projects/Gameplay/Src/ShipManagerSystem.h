#pragma once

#include <EntitySystem.h>
#include <AglMatrix.h>
#include <AglBoundingSphere.h>

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
struct FreeSlotData
{
	Entity* parent;
	AglMatrix offset;
	int index;
};


class ShipManagerSystem : public EntitySystem
{
private:
	vector<FreeSlotData> findFreeConnectionPoints(Entity* p_entity, int p_parent);
public:
	ShipManagerSystem();
	const vector<Entity*>& getShips() const;
	Entity* findShip(int p_ownerId);
	vector<FreeSlotData> findFreeConnectionPoints(int p_ownerId);
	AglBoundingSphere findEditSphere(int p_ownerId);

};
