#pragma once

#include "OnHitScoreEffectPacket.h"
#include <queue>
#include <Entity.h>
#include <EntitySystem.h>
#include "ModuleStatusEffectPacket.h"

using namespace std;

class TcpServer;

// =======================================================================================
//                             ModuleVisualEffectBufferSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System that buffers effect packets for sendout and visualization to clients 
///        
/// # OnHitEffectBufferSystem
/// Detailed description.....
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------

class ModuleVisualEffectBufferSystem : public EntitySystem
{
public:
	ModuleVisualEffectBufferSystem(TcpServer* p_server);
	virtual ~ModuleVisualEffectBufferSystem();

	virtual void initialize();
	virtual void process();

	void enqueueEffect(Entity* p_entity, OnHitScoreEffectPacket& p_packet);
	void enqueueEffect(int p_networkOwnerId, OnHitScoreEffectPacket& p_packet);
	void enqueueEffect(ModuleStatusEffectPacket& p_packet);
protected:
private:
	queue<pair<Entity*,OnHitScoreEffectPacket>> m_scoreFXqueue_entity;
	queue<pair<int,OnHitScoreEffectPacket>> m_scoreFXqueue_netowner;
	queue<ModuleStatusEffectPacket> m_statusFXqueue_netowner;
	TcpServer* m_server;
};