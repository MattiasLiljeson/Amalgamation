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
	virtual void initialize();
	virtual void processEntities( const vector<Entity*>& p_entities );

	virtual void inserted( Entity* p_entity );

};

