#pragma once
#include <EntitySystem.h>
#include "NetworkSynced.h"

class NetworkSynced;
class PlayerComponent;

// =======================================================================================
//                                      PlayerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles all the players
///        
/// # PlayerSystem
/// Detailed description.....
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------

class PlayerSystem : public EntitySystem
{
public:
	PlayerSystem();
	~PlayerSystem();
	PlayerComponent* getPlayerCompFromNetworkComp(NetworkSynced* p_netComponent);
	const vector<PlayerComponent*>& getPlayerComponents() const;
	string getPlayerNameFromID(int p_playerID) const;
	void inserted( Entity* p_entity );
private:
	vector<PlayerComponent*> m_playerComponents;
};