#pragma once

#include <EntitySystem.h>

class UpdateClientStatsPacket;
class DisplayGameStats;
class PlayersWinLosePacket;

// =======================================================================================
//                                      ScoreDataTableSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ScoreDataTableSystem
/// Detailed description.....
/// Created on: 23-1-2013 
///---------------------------------------------------------------------------------------

class GameStatsSystem : public EntitySystem
{
public:
	GameStatsSystem();

	void initialize();

	virtual ~GameStatsSystem();

	void updateStats(const UpdateClientStatsPacket* p_packet);
	void updateResults(const PlayersWinLosePacket* p_packet);

	virtual void process();

private:
	int						m_rocketDocument;
	bool					m_infoPanelVisible;
	DisplayGameStats*		m_infoPanel;
};