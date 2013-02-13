#pragma once
#include <EntitySystem.h>
// =======================================================================================
// ShieldPlatingSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ShieldPlatingSystem
/// Detailed description...
/// Created on: 12-2-2013 
///---------------------------------------------------------------------------------------
class ShieldPlatingSystem: public EntitySystem
{
public:
	ShieldPlatingSystem();
	void inserted( Entity* p_entity );
	void processEntities( const vector<Entity*>& p_entities );

};