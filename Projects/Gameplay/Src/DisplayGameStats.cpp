#include "DisplayGameStats.h"
#include <ToString.h>

DisplayGameStats::DisplayGameStats(const char* p_name, const char* p_tableName)
	: DataSource(p_name)
{
	m_tableName = p_tableName;
	for (int i = 0; i < MAXPLAYERS; i++)
	{
		m_players[i].name	= "Vadarettkulnamn?";
		m_players[i].score	= 0;
		m_players[i].ping	= -1;
	}
	m_activePlayers = 0;
	m_dirty = false;
	//NotifyRowAdd("infopanel", 0, MAXPLAYERS);
}

DisplayGameStats::~DisplayGameStats()
{

}

void DisplayGameStats::GetRow( Rocket::Core::StringList& row, 
							  const Rocket::Core::String& table,
							  int row_index, 
							  const Rocket::Core::StringList& columns )
{
	if (table == m_tableName)
	{
		for (int col_index = 0; col_index < columns.size(); col_index++)
		{
			if (columns[col_index] == "playerName")
			{
				//row.push_back(Rocket::Core::String("playerName"));
				row.push_back(m_players[row_index].name.c_str());
			}
			else if (columns[col_index] == "score")
			{
				//row.push_back(Rocket::Core::String("score"));
				row.push_back(toString((int)(m_players[row_index].score+0.5f)).c_str());
			}
			else if (columns[col_index] == "ping")
			{
				//row.push_back(Rocket::Core::String("ping"));;
				row.push_back(toString(m_players[row_index].ping).c_str());
			}
		}
	}
}

int DisplayGameStats::GetNumRows( const Rocket::Core::String& table )
{
	if (table == m_tableName)
	{
		return m_activePlayers;
	}
	return 0;
}

void DisplayGameStats::updateRow( int p_row, const PlayerStats& p_stats )
{
	m_players[p_row].name	= p_stats.name;
	m_players[p_row].ping	= p_stats.ping;
	m_players[p_row].score	= p_stats.score;

	m_dirty = true;
}

void DisplayGameStats::updateTheVisualInfoPanel()
{
	if (m_dirty)
	{
		NotifyRowChange(m_tableName, 0, m_activePlayers);
		m_dirty = false;
	}
}

void DisplayGameStats::setActivePlayers( int p_players )
{
	m_activePlayers = p_players;
}

int DisplayGameStats::getActivePlayers() const
{
	return m_activePlayers;
}

void DisplayGameStats::addRows( int p_nrOfNewRows )
{
	NotifyRowAdd(m_tableName, m_activePlayers, p_nrOfNewRows);
}

void DisplayGameStats::removeRows( int p_nrOfRows )
{
	NotifyRowRemove(m_tableName, m_activePlayers - p_nrOfRows, p_nrOfRows);
}

