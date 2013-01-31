#pragma once
#include <EntitySystem.h>

#define WINAPI
// =======================================================================================
//                                      ServerClientInfoSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ServerClientInfoSystem
/// Detailed description.....
/// Created on: 24-1-2013 
///---------------------------------------------------------------------------------------

class ServerClientInfoSystem : public EntitySystem
{
public:
	ServerClientInfoSystem();
	virtual ~ServerClientInfoSystem();

	virtual void inserted( Entity* p_entity );

	virtual void removed( Entity* p_entity );

	virtual void processEntities( const vector<Entity*>& p_entities );



private:
};