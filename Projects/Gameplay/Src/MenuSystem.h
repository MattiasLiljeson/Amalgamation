#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                      MenuSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # MenuSystem
/// Detailed description.....
/// Created on: 4-2-2013 
///---------------------------------------------------------------------------------------

class MenuSystem : public EntitySystem
{
public:
	MenuSystem();
	~MenuSystem();
	void initialize();
	void process( );
	void endLoadingState();
	void displayDisconnectPopup();
	void setJoin();
	void setHost();
private:
	int m_loadingWindowIdx;
	int m_disconnectPopupIdx;
	int m_lobbyDocIdx;
	int m_joinIdx;
	int m_hostIdx;
};

