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
		while(!m_queue_entity.empty())
		{
			auto infoPair = m_queue_entity.front();

			auto netSync = static_cast<NetworkSynced*>(
				infoPair.first->getComponent(ComponentType::NetworkSynced));

			m_server->unicastPacket(infoPair.second.pack(), 
				netSync->getNetworkOwner());

			m_queue_entity.pop();
		}
		while(!m_queue_netowner.empty())
		{
			auto infoPair = m_queue_netowner.front();

			if (infoPair.first!=-1)
			{
				m_server->unicastPacket(infoPair.second.pack(), 
					infoPair.first);
			}


			m_queue_entity.pop();
		}
	}
}


void OnHitEffectBufferSystem::enqueueEffect( Entity* p_entity, 
											OnHitScoreEffectPacket& p_packet )
{
	m_queue_entity.push(pair<Entity*,OnHitScoreEffectPacket>(p_entity,p_packet));
}

void OnHitEffectBufferSystem::enqueueEffect(int p_networkOwnerId, 
											OnHitScoreEffectPacket& p_packet)
{
	m_queue_netowner.push(pair<int,OnHitScoreEffectPacket>(p_networkOwnerId,p_packet));
}
