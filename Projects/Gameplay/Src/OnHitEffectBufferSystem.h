#pragma once

#include "OnHitScoreEffectPacket.h"
#include <queue>
#include <Entity.h>
#include <TcpServer.h>
#include <EntitySystem.h>

using namespace std;

// =======================================================================================
//                             OnHitEffectBufferSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	System that buffers onhiteffect packets for sendout
///        
/// # OnHitEffectBufferSystem
/// Detailed description.....
/// Created on: 13-2-2013 
///---------------------------------------------------------------------------------------

class OnHitEffectBufferSystem : public EntitySystem
{
public:
	OnHitEffectBufferSystem(TcpServer* p_server);
	virtual ~OnHitEffectBufferSystem();

	virtual void initialize();
	virtual void process();

	void enqueueEffect(Entity* p_entity, OnHitScoreEffectPacket& p_packet);
	void enqueueEffect(int p_networkOwnerId, OnHitScoreEffectPacket& p_packet);
protected:
private:
	queue<pair<Entity*,OnHitScoreEffectPacket>> m_queue_entity;
	queue<pair<int,OnHitScoreEffectPacket>> m_queue_netowner;
	TcpServer* m_server;
};