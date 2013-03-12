#pragma once
#include <EntitySystem.h>
// =======================================================================================
// ClientModuleCounterSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # ClientModuleCounterSystem
/// Detailed description...
/// Created on: 8-3-2013 
///---------------------------------------------------------------------------------------
class ClientModuleCounterSystem: public EntitySystem
{
public:
	ClientModuleCounterSystem();
	void processEntities( const vector<Entity*>& p_entities ) final;
	void initialize() final;

private:
	unsigned int m_numberOfModules;
};