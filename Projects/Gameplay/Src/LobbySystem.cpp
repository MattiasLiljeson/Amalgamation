#include "LobbySystem.h"
#include <ToString.h>

LobbySystem::LobbySystem() : EntitySystem (SystemType::LobbySystem), 
	DataSource("connectedPlayers")
{
	m_tableName = "lobbyPanel";

	for(int i = 0; i < MAXPLAYERS; i++)
	{
		m_players[i].playerName = "open";
		m_players[i].ping = -1;
		m_players[i].ready = false;

	}
	m_connectPlayers = 0;
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
		/*
	if (table == m_tableName)
	{
		
		int numberOfPlayers = 0;
		for(unsigned int i=0;i < MAXPLAYERS;i++){
			if(m_players[i].playerName != "")
				numberOfPlayers++;
		}
		return numberOfPlayers;
	}*/
	return MAXPLAYERS;
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

void LobbySystem::addNewPlayer( const NewlyConnectedPlayerPacket& p_packet )
{
	m_players[p_packet.playerID].playerName = p_packet.playerName;
	m_players[p_packet.playerID].ping = p_packet.playerID;
	m_players[p_packet.playerID].ready = false;

	NotifyRowChange(m_tableName,p_packet.playerID,1);
}