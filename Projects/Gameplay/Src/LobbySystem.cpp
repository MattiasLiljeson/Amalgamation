#include "LobbySystem.h"
#include <ToString.h>

LobbySystem::LobbySystem() : EntitySystem (SystemType::LobbySystem), 
	DataSource("connectedPlayers")
{
	m_tableName = "lobbyPanel";

	for(int i = 0; i < MAXPLAYERS; i++)
	{
		m_players[i].playerName = "Roffeltord";
		m_players[i].ping = i;
		m_players[i].ready = false;

	}

	NotifyRowAdd(m_tableName,0,MAXPLAYERS);
}

LobbySystem::~LobbySystem()
{

}

void LobbySystem::GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, 
						 int row_index, const Rocket::Core::StringList& columns )
{
	if(table == m_tableName){
		for (int col_index = 0; col_index < columns.size(); col_index++)
		{
			if (columns[col_index] == "playerName")
			{
				//row.push_back(Rocket::Core::String("playerName"));
				row.push_back(m_players[row_index].playerName.c_str());
			}
			else if (columns[col_index] == "ping")
			{
				//row.push_back(Rocket::Core::String("score"));
				row.push_back(toString(m_players[row_index].ping).c_str());
			}
			else if (columns[col_index] == "ready")
			{
				//row.push_back(Rocket::Core::String("ping"));;
				row.push_back(toString(m_players[row_index].ready).c_str());
			}
		}
	}
}

int LobbySystem::GetNumRows( const Rocket::Core::String& table )
{
	if (table == m_tableName)
	{
		return MAXPLAYERS;
	}
	return 0;
}

void LobbySystem::process()
{

}

void LobbySystem::initialize()
{

}

void LobbySystem::updateRow( int p_row, const LobbyStats& p_stats )
{

}
