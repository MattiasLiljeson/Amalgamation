#pragma once
#include <EntitySystem.h>
class PreciseTimer;
// =======================================================================================
// ClientEntityCountSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Statistics about the number of entities in the game.
///        
/// # ClientEntityCountSystem
/// Counts the total number of Entities.
/// Tracks the creation rate.
/// Tracks the deletion rate.
/// Created on: 8-2-2013 
///---------------------------------------------------------------------------------------
class ClientEntityCountSystem: public EntitySystem
{
public:
	ClientEntityCountSystem();
	~ClientEntityCountSystem();
	void initialize();
	void process();
	void added( Entity* p_entity );
	void deleted( Entity* p_entity );

private:
	PreciseTimer* m_secondTimer;
	// The values to fetch
	unsigned int m_entitiesTotal;
	unsigned int m_entitiesInsertedLastSecond;
	unsigned int m_entitiesRemovedLastSecond;
	// Counters
	unsigned int m_entitiesInserted;
	unsigned int m_entitiesRemoved;
};