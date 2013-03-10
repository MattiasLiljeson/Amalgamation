#pragma once
#include <EntitySystem.h>
#include <Rocket/Controls/DataSource.h>
#include <string>
#include <Globals.h>
#include "NewlyConnectedPlayerPacket.h"
#include "DisconnectPacket.h"

using namespace Rocket::Controls;

// =======================================================================================
//                                      LobbySystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # LobbySystem
/// Detailed description.....
/// Created on: 20-2-2013 
///---------------------------------------------------------------------------------------

class LobbySystem : public EntitySystem, private DataSource
{
public:
	struct LobbyStats
	{
		std::string playerName;
		int ping;
		bool ready;
	};
public:
	LobbySystem();
	~LobbySystem();

	void GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, 
		int row_index, const Rocket::Core::StringList& columns );

	int GetNumRows( const Rocket::Core::String& table );

	void process();

	void initialize();
	void updateRow(int p_row, const LobbyStats& p_stats);
	void addNewPlayer(const NewlyConnectedPlayerPacket& p_packet);
	void removePlayer(const DisconnectPacket& p_packet);
	void resetAllPlayers();
private:
	void resetPlayer(int p_playerId);
private:
	const char* m_tableName;
	int m_connectPlayers;
	LobbyStats m_players[MAXPLAYERS];
};