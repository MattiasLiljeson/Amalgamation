#pragma once
#include <EntitySystem.h>
#include "NetworkSynced.h"
#include <UniqueIndexList.h>


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
	string getPlayerNameFromID(int p_playerID);
	void inserted( Entity* p_entity );
	void removed( Entity* p_entity );

	//PlayerComponent* serverCreatePlayerComponent(int p_fromNetworkOwnerId);
	void deletePlayerEntity(int p_playerId);
	void deleteAllPlayerEntities();

	int createPlayerId(int p_fromNetworkOwnerId);
	int findPlayerId(int p_fromNetworkOwnerId);
	PlayerComponent* findPlayerComponentFromNetworkID( int p_fromNetworkOwnerId );
	PlayerComponent* findPlayerComponentFromPlayerID( int p_playerId );
	void recyclePlayerId(int p_playerId);
private:
	vector<PlayerComponent*> m_playerComponents;

	UniqueIndexList<int> m_playerIds;
};