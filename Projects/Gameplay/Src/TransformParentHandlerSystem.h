#pragma once

#include <EntitySystem.h>

// =======================================================================================
//                               TransformParentHandlerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # TransformParentHandlerSystem
/// Detailed description.....
/// Created on: 21-1-2013 
///---------------------------------------------------------------------------------------

class EntityParentHandlerSystem : public EntitySystem
{
public:
	EntityParentHandlerSystem();
	~EntityParentHandlerSystem();

	virtual void inserted( Entity* p_entity ) final;
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

private:
	bool checkDebugInformation(int p_child, int p_parent);

private:
	map<int, int> m_entityHierarchy; // <child index, parent index>

};