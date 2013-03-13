#include "LobbySystem.h"
#include <ToString.h>

LobbySystem::LobbySystem() : EntitySystem (SystemType::LobbySystem), 
	DataSource("connectedPlayers")
{
	m_tableName = "lobbyPanel";

	for(int i = 0; i < MAXPLAYERS; i++)
	{
		resetPlayer(i);
	}
	m_connectPlayers = 0;
	NotifyRowAdd(m_tableName,0,MAXPLAYERS);
}

void LobbySystem::resetPlayer( int p_playerId )
{
	m_players[p_playerId].playerName	= "open";
	m_players[p_playerId].ping			= -1;
	m_players[p_playerId].ready			= false;
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
				if(m_players[row_index].ready){
					row.push_back("Yes");
				}
				else{
					row.push_back("No");
				}
			}
		}
	}
}

int LobbySystem::GetNumRows( const Rocket::Core::String& table )
{
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
	m_players[p_row].playerName = p_stats.playerName;
	m_players[p_row].ping		= p_stats.ping;
	m_players[p_row].ready		= p_stats.ready;
	NotifyRowChange(m_tableName, p_row, 1);
}

void LobbySystem::setPlayerReady( int p_playerId, bool p_ready )
{
	m_players[p_playerId].ready	= p_ready;
	NotifyRowChange(m_tableName, p_playerId, 1);
}

void LobbySystem::addNewPlayer( const NewlyConnectedPlayerPacket& p_packet )
{
	m_players[p_packet.playerID].playerName = p_packet.playerName;
	m_players[p_packet.playerID].ping		= p_packet.playerID;
	m_players[p_packet.playerID].ready		= false;

	NotifyRowChange(m_tableName,p_packet.playerID, 1);
}

void LobbySystem::removePlayer( const DisconnectPacket& p_packet )
{
	resetPlayer(p_packet.playerID);
	NotifyRowChange(m_tableName, p_packet.playerID, 1);
}

void LobbySystem::resetAllPlayers()
{
	for(int i = 0; i < MAXPLAYERS; i++)
	{
		resetPlayer(i);
	}
	NotifyRowChange(m_tableName,0,MAXPLAYERS);
}

bool LobbySystem::isPlayerReady( int p_playerId ) const
{
	return m_players[p_playerId].ready;
}

void LobbySystem::setAllPlayersReady( bool p_ready )
{
	for(int i = 0; i < MAXPLAYERS; i++)
	{
		m_players[i].ready = p_ready;
	}
	NotifyRowChange(m_tableName,0,MAXPLAYERS);
}


