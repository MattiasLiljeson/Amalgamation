#pragma once

#include <EntitySystem.h>

class UpdateClientStatsPacket;
class DisplayGameStats;

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

	virtual void process();

protected:
private:
	int						m_infoPanelDoc;
	bool					m_displayInfoPanel;
	DisplayGameStats*		m_infoPanel;
};