#pragma once

#include <EntitySystem.h>

// =======================================================================================
//	DisplayPlayerScoreSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Prints players' score using anttweakbar.
///        
/// # DisplayPlayerScoreSystem
/// Detailed description...
/// Created on: 4-1-2013 
///---------------------------------------------------------------------------------------

class DisplayPlayerScoreSystem: public EntitySystem
{
public:
	DisplayPlayerScoreSystem();

	~DisplayPlayerScoreSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void initialize();

	void added( Entity* p_entity );

};