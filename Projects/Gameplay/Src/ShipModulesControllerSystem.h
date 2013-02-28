#pragma once

#include <EntitySystem.h>
#include "AglVector3.h"
#include "ConnectionPointSet.h"

class Transform;
class TcpServer;
class ModuleVisualEffectBufferSystem;

// =======================================================================================
//                                      ShipModulesControllerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System for handling communication with the physics library
///        
/// # ShipModulesControllerSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------

class ShipModulesControllerSystem: public EntitySystem
{
public:
	ShipModulesControllerSystem(TcpServer* p_server,
		ModuleVisualEffectBufferSystem* p_effectBuffer);
	~ShipModulesControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void applyImpulse(int p_bodyID, AglVector3 p_impulse, AglVector3 p_angularImpulse);

	void addHighlightEvent(int p_slot, int p_id,int p_status=1);
	void addActivateEvent(int p_index);
	void addDeactivateEvent(int p_index);
	void setEditMode(bool p_editMode);

private:
	void checkDrop_ApplyScoreAndDamage(Entity* p_parent);
	void drop(Entity* p_parent, unsigned int p_slot);
	void changeHighlight(Entity* p_entity, int p_new, int p_status);
	void setActivation(Entity* p_entity, bool p_value);
	void setActivationChildren(Entity* p_entity, bool p_value);
	// float calculateScore(Entity* p_entity);

	// effect visualization for client
	void setScoreEffect(int p_networkOwner, Transform* p_moduleTransform, int p_score);
	void enableModuleUnusuedEffect(int p_moduleNetworkOwner);
	void disableModuleUnusuedEffect(int p_moduleNetworkOwner);
	void updateModuleHealthEffect(int p_moduleNetworkOwner, float p_healthPercent);
	void updateModuleValueEffect(int p_moduleNetworkOwner, float p_valuePercent);
private:
	struct HighlightEvent
	{		
		int id;
		int slot;
		int status;
	};

	vector<HighlightEvent> m_toHighlight;
	vector<int> m_toActivate;
	vector<int> m_toDeactivate;

	// system dependencies
	TcpServer* m_server;
	ModuleVisualEffectBufferSystem* m_effectbuffer;

	bool m_editMode;

};