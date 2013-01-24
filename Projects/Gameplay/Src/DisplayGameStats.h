#pragma once

#include <Rocket/Controls/DataSource.h>
#include <Globals.h>
#include "PlayerStats.h"

using namespace Rocket::Controls;

// =======================================================================================
//                                      DisplayGameStats
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # DisplayGameStats
/// Detailed description.....
/// Created on: 24-1-2013 
///---------------------------------------------------------------------------------------

class DisplayGameStats : public DataSource
{
public:
	DisplayGameStats(const char* p_name, const char* p_tableName);
	virtual ~DisplayGameStats();

	void GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, 
				int row_index, const Rocket::Core::StringList& columns );

	int GetNumRows( const Rocket::Core::String& table );

	void addRows(int p_nrOfNewRows);
	void removeRows(int p_nrOfNewRows);

	void updateRow(int p_row, const PlayerStats& p_stats);
	void updateTheVisualInfoPanel();

	int getActivePlayers() const;
	void setActivePlayers(int p_players);
protected:
private:
	const char* m_tableName;
	int m_activePlayers;
	PlayerStats m_players[MAXPLAYERS];
};