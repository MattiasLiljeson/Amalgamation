#pragma once
#include <EntitySystem.h>
class InputBackendSystem;
class TcpClient;
// =======================================================================================
// ClientDebugModuleSpawnerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Being able to send create-module packet to server.
///        
/// # ClientDebugModuleSpawnerSystem
/// Detailed description...
/// Created on: 14-3-2013 
///---------------------------------------------------------------------------------------
class ClientDebugModuleSpawnerSystem: public EntitySystem
{
public:
	ClientDebugModuleSpawnerSystem(TcpClient* p_client);
	void processEntities( const vector<Entity*>& p_entities ) final;
	void initialize() final;

private:
	InputBackendSystem* m_input;
	TcpClient* m_client;
};