#pragma once
#include <EntitySystem.h>
#include <Rocket/Controls/DataSource.h>
#include <string>
#include <Globals.h>

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

private:
	const char* m_tableName;
	LobbyStats m_players[MAXPLAYERS];
};