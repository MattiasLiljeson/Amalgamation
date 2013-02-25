#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                                      LevelHandlerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Provides handling of a created level.
///        
/// # LevelHandlerSystem
/// This system keeps track of all entities that have a LevelPieceRoot component. The
/// main purpose of the system is to initialize new pieces with right transform data, and
/// to be able to destroy a level easily.
/// Created on: 14-2-2013 
///---------------------------------------------------------------------------------------

class LevelHandlerSystem : public EntitySystem
{
public:
	LevelHandlerSystem();
	~LevelHandlerSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void inserted( Entity* p_entity );

	void destroyLevel(); // Destroys the level and clears all spawnpoints
};