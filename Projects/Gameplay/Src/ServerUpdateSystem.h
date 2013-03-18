#pragma once

#include <EntitySystem.h>
#include "Transform.h"
#include "AglParticleSystemHeader.h"
#include "ParticleSystemServerComponent.h"
class TcpServer;

// =======================================================================================
//                                      ServerUpdateSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkUpdateSystem
/// Detailed description.....
/// Created on: 20-12-2012 
///---------------------------------------------------------------------------------------

class ServerUpdateSystem : public EntitySystem
{
public:
	ServerUpdateSystem( TcpServer* p_server );

	~ServerUpdateSystem();
	
	void processEntities( const vector<Entity*>& p_entities );

	void initialize();

	virtual void removed( Entity* p_entity );
private:
	void sendPlayerStats();

private:
	TcpServer*	m_server;
	float		m_timestamp;

	struct TransformRecord {
		Transform transform;
		float timestamp;
	};
	map<int, TransformRecord> m_previousTransforms;

	struct ParticleRecord {
		AglParticleSystemHeader particleHeader;
		float timestamp;
	};
	map<AglParticleSystemHeader*, ParticleRecord> m_previousParticles;
};