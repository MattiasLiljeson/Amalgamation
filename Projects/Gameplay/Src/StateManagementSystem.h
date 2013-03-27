#pragma once

#include <EntitySystem.h>

class TcpClient;
// =======================================================================================
//                                      StateManagementSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Manages how entities and components are created and deleted during states.
///        
/// # StateManagementSystem
/// Detailed description.....
/// Created on: 14-3-2013 
///---------------------------------------------------------------------------------------

class StateManagementSystem : public EntitySystem
{
public:
	StateManagementSystem(TcpClient* p_client);
	
	virtual ~StateManagementSystem();

	virtual void process();

	virtual void initialize();

protected:
private:
	TcpClient* m_client;
};