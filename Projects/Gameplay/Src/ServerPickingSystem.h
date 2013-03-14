#pragma once

#include <EntitySystem.h>
#include "Transform.h"
#include "PickComponent.h"
#include "ConnectionPointSet.h"
#include "ModuleVisualEffectServerBufferSystem.h"

class ShipModule;
class PlayerSystem;
class TcpServer;

// =======================================================================================
//                                      ServerPickingSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Creates rays and sends to server
///        
/// # ServerPickingSystem
/// Detailed description.....
/// Created on: 11-12-2012 
///---------------------------------------------------------------------------------------

class ServerPickingSystem: public EntitySystem
{
public:
	ServerPickingSystem(TcpServer* p_server,
		ModuleVisualEffectServerBufferSystem* p_effectBuffer);
	~ServerPickingSystem();

	virtual void initialize();
	void processEntities(const vector<Entity*>& p_entities );

	void setRay(int p_index, AglVector3 p_o, AglVector3 p_d);
	void setEnabled(int p_index, bool p_value);
	void setReleased(int p_index);

	//Rotation event
	void addRotationEvent(int direction, int client);
	void add90RotationEvent(int direction, int client);

	//Toggle preferred slot event
	void togglePreferredSlot(int client);

private:
	vector<PickComponent> m_pickComponents;
	TcpServer* m_server;

	PlayerSystem* m_playerSystem;

	//Temp
	float mrota;
private:
	void handleRay(PickComponent& p_pc, const vector<Entity*>& p_entities);
	void project(Entity* toProject, PickComponent& p_ray);
	AglVector3 closestConnectionPoint(AglVector3 p_position, Entity* p_entity, PickComponent& p_pc);
	void attemptConnect(PickComponent& p_ray);
	bool attemptDetach(PickComponent& p_ray);
	AglMatrix offsetTemp(Entity* p_entity, AglMatrix p_base, AglMatrix p_offset, float p_rotation = 0);
	void setScoreEffect(Entity* p_player, Transform* p_moduleTransform, int p_score);
	void setModuleUsedStatusEffect(Entity* p_module);
	vector<pair<int, Entity*>> getFreeConnectionPoints(ConnectionPointSet* p_set, Entity* p_parent);

	//Rotation of modules
	void rotateModule(Entity* p_ship, int p_dir);

	AglVector3 project(AglVector3 p_o, AglVector3 p_d, AglVector3 p_point);

	void updateModuleHealthEffect( int p_moduleNetworkOwner, float p_healthPercent );

	void updateModuleValueEffect( int p_moduleNetworkOwner, float p_valuePercent );

	//Send information about the Selection marker
	void updateSelectionMarker(PickComponent& p_ray);

	void unsetPick(PickComponent& p_ray);

	// Effects
	ModuleVisualEffectServerBufferSystem* m_effectbuffer;
};