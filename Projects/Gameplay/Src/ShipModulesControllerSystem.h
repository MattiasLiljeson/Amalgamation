#pragma once

#include <EntitySystem.h>
#include "AglVector3.h"
#include "ConnectionPointSet.h"

class Transform;
class TcpServer;
class OnHitEffectBufferSystem;

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


struct RotationState
{
	int direction;
	int targetShip;
	RotationState(int p_direction, int p_targetShip)
	{
		direction = p_direction;
		targetShip = p_targetShip;
	}
};

class ShipModulesControllerSystem: public EntitySystem
{
public:
	ShipModulesControllerSystem(TcpServer* p_server,
		OnHitEffectBufferSystem* p_effectBuffer);
	~ShipModulesControllerSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void applyImpulse(int p_bodyID, AglVector3 p_impulse, AglVector3 p_angularImpulse);

	void addHighlightEvent(int p_slot, int p_id,int p_status=1);
	void addActivateEvent(int p_index);
	void addDeactivateEvent(int p_index);

	void addRotationEvent(RotationState p_rotationState);

private:
	void checkDrop(Entity* p_parent);
	void drop(Entity* p_parent, unsigned int p_slot);
	void changeHighlight(Entity* p_entity, int p_new, int p_status);
	void setActivation(Entity* p_entity, bool p_value);
	void setActivationChildren(Entity* p_entity, bool p_value);
	float calculateScore(Entity* p_entity);

	AglMatrix offsetTemp(Entity* p_entity, AglMatrix p_base, AglMatrix p_offset, float p_rotation);

	void rotateModules(Entity* p_ship);
	void setRotationState(Entity* p_ship, int p_state);
	void setScoreEffect(int p_networkOwner, Transform* p_moduleTransform, int p_score);

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

	vector<RotationState> m_toSetRotationState;

	// system dependencies
	TcpServer* m_server;
	OnHitEffectBufferSystem* m_effectbuffer;
};