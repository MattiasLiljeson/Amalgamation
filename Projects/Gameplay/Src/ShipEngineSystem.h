#pragma once
#include <EntitySystem.h>
// =======================================================================================
//                                      ShipEngineSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # ShipEngineSystem
/// Detailed description.....
/// Created on: 7-3-2013 
///---------------------------------------------------------------------------------------

class ShipEngineSystem : public EntitySystem
{
public:
	ShipEngineSystem();
	~ShipEngineSystem();

	void processEntities( const vector<Entity*>& p_entities );

	void inserted( Entity* p_entity );

	void removed( Entity* p_entity );

protected:
private:
	unsigned int m_shipIdleSoundIndex;
	unsigned int m_shipActiveSoundIndex;
	
};