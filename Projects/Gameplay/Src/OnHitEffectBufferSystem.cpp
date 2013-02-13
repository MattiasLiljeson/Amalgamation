#include "OnHitEffectBufferSystem.h"
#include "TimerSystem.h"
#include "NetworkSynced.h"

OnHitEffectBufferSystem::OnHitEffectBufferSystem(TcpServer* p_server) :
	EntitySystem(SystemType::OnHitEffectBufferSystem)
{
	m_server = p_server;
}

OnHitEffectBufferSystem::~OnHitEffectBufferSystem()
{

}

void OnHitEffectBufferSystem::initialize()
{

}

void OnHitEffectBufferSystem::process()
{
	// Dequeue all packets and send to their clients
	if(static_cast<TimerSystem*>(m_world->getSystem(SystemType::TimerSystem))->
		checkTimeInterval(TimerIntervals::Every8Millisecond))
	{
		while(!m_queue.empty())
		{
			auto infoPair = m_queue.front();

			auto netSync = static_cast<NetworkSynced*>(
				infoPair.first->getComponent(ComponentType::NetworkSynced));

			m_server->unicastPacket(infoPair.second.pack(), 
				netSync->getNetworkIdentity());

			m_queue.pop();
		}
	}
}


void OnHitEffectBufferSystem::enqueueEffect( Entity* p_entity, 
											OnHitScoreEffectPacket& p_packet )
{
	m_queue.push(pair<Entity*,OnHitScoreEffectPacket>(p_entity,p_packet));
}

