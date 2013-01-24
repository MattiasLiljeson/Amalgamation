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
	DisplayGameStats(const char* p_name);
	virtual ~DisplayGameStats();

	void GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, 
				int row_index, const Rocket::Core::StringList& columns );

	int GetNumRows( const Rocket::Core::String& table );

	void updateRow(int p_row, const PlayerStats& p_stats);
	void updateTheVisualInfoPanel();
protected:
private:
	PlayerStats m_players[MAXPLAYERS];
};