#pragma once

#include <EntitySystem.h>
#include <vector>

using namespace std;

// =======================================================================================
//                                      ServerStaticObjectsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles all the static props located on the server.
///        
/// # ServerStaticObjectsSystem
/// Detailed description.....
/// Created on: 11-1-2013 
///---------------------------------------------------------------------------------------

class ServerStaticObjectsSystem : public EntitySystem
{
public:
	ServerStaticObjectsSystem();
	~ServerStaticObjectsSystem();

	void processEntities(const vector<Entity*>& p_entities );

	void initialize();

	//void added( Entity* p_entity );
	
	vector<Entity*> getStaticObjects();


private:
	//vector<Entity*> m_objects;
};