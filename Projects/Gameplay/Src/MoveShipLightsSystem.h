#pragma once
#include <EntitySystem.h>

// =======================================================================================
//	MoveShipLightsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief description...
///        
/// # MoveShipLightsSystem
/// Detailed description...
/// Created on: 25-1-2013 
///---------------------------------------------------------------------------------------

class MoveShipLightsSystem: public EntitySystem
{
public:
	MoveShipLightsSystem();
	~MoveShipLightsSystem();
	void initialize();
	void processEntities( const vector<Entity*>& p_entities );

private:
	float m_lightPosition[3];

};