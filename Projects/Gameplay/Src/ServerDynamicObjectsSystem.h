#pragma once

#include <EntitySystem.h>
#include <vector>

// =======================================================================================
//                                      ServerDynamicObjectsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles all the dynamic entities
///        
/// # ServerDynamicObjectsSystem
/// Detailed description.....
/// Created on: 11-1-2013 
///---------------------------------------------------------------------------------------

class ServerDynamicObjectsSystem : public EntitySystem
{
public:
	ServerDynamicObjectsSystem();
	~ServerDynamicObjectsSystem();

	void processEntities(const vector<Entity*>& p_entities );

	void initialize();

	void added( Entity* p_entity );
	vector<Entity*> getDynamicObjects();

	void deleted( Entity* p_entity );

private:
	vector<Entity*> m_objects;
};