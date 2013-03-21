#pragma once
#include <EntitySystem.h>
// =======================================================================================
// ModuleCleaningService
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Removes modules after being "free" for too long.
///        
/// # ModuleCleaningService
/// Detailed description...
/// Created on: 18-3-2013 
///---------------------------------------------------------------------------------------
class ModuleCleaningService: public EntitySystem
{
public:
	ModuleCleaningService();
	~ModuleCleaningService();
	void processEntities( const vector<Entity*>& p_entities );
	void inserted( Entity* p_entity );

};