#pragma once
#include <EntitySystem.h>
// =======================================================================================
// ShipHighlightSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ShipHighlightSystem
/// Detailed description...
/// Created on: 25-2-2013 
///---------------------------------------------------------------------------------------
class ShipHighlightSystem: public EntitySystem
{
private:
	float m_minDistance;
	float m_maxDistance;
public:
	ShipHighlightSystem();
	virtual void processEntities( const vector<Entity*>& p_entities ) final;

	float* getMinDistance();
	float* getMaxDistance();

};