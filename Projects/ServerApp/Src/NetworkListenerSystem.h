// =======================================================================================
//                                      NetworkListenerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # NetworkListenerSystem
/// Detailed description.....
/// Created on: 3-12-2012 
///---------------------------------------------------------------------------------------
#pragma once

#include <SystemType.h>
#include <EntitySystem.h>
#include <TcpListenerProcess.h>

class NetworkListenerSystem: public EntitySystem
{
public:
	NetworkListenerSystem();
	~NetworkListenerSystem();

	virtual void process();

	virtual void initialize();



};