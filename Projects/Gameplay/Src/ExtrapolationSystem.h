#pragma once

#include <AntTweakBarWrapper.h>
#include <EntitySystem.h>
class TcpClient;

// =======================================================================================
//	ExtrapolationSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief A system that is supposed to extrapolate the position of entities with the
/// Extrapolate component.
///
/// # ExtrapolationSystem
/// First synchronizing the older positional updates with the latest update so that all
/// entities have the same local update time stamp. The difference in update time is also
/// stored which the extrapolation uses.
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class ExtrapolationSystem: public EntitySystem
{
public:
	ExtrapolationSystem( TcpClient* p_client );

	~ExtrapolationSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void initialize();
	///-----------------------------------------------------------------------------------
	/// Find the latest update packet time stamp that can be compared with every entity to
	/// be extrapolated.
	/// \param p_entities
	/// \return float
	///-----------------------------------------------------------------------------------
	float searchForLatestUpdateTimeStamp( const vector<Entity*>& p_entities );
private:
	static void TW_CALL togglePositionalExtrapolation(void* p_clientData);

private:
	TcpClient* m_client;
	float m_correctedDeltaTime;
	bool m_usePositionalExtrapolation;
	static ExtrapolationSystem* m_selfPointer;

};