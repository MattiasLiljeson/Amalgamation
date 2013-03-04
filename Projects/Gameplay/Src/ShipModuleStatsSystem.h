#pragma once

#include <EntitySystem.h>
#include "ModuleVisualEffectServerBufferSystem.h"

// =======================================================================================
//                               ShipModuleStatsSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Simple system for per module-based actions. Currently used to sent regular
/// updates to client of module stats, for effect visualization.
///        
/// # ShipModuleStatsSystem
/// Detailed description.....
/// Created on: 27-2-2013 
///---------------------------------------------------------------------------------------

class ShipModuleStatsSystem : public EntitySystem
{
public:
	ShipModuleStatsSystem(ModuleVisualEffectServerBufferSystem* p_effectBuffer);
	virtual ~ShipModuleStatsSystem();

	void initialize();
	void inserted( Entity* p_entity );
	void removed( Entity* p_entity );
	void processEntities(const vector<Entity*>& p_entities );
protected:
private:
	void enableModuleUnusuedEffect(int p_moduleNetworkOwner);
	void disableModuleUnusuedEffect(int p_moduleNetworkOwner);
	void updateModuleHealthEffect(int p_moduleNetworkOwner, float p_healthPercent);
	void updateModuleValueEffect(int p_moduleNetworkOwner, float p_valuePercent);
	void updateModuleFreeFloatEffect(int p_moduleNetworkOwner, bool p_mode);

	static unsigned int m_currentStart;
	static const unsigned int m_batchSz=10;
	ModuleVisualEffectServerBufferSystem* m_effectbuffer;
};