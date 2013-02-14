#pragma once
#include <EntitySystem.h>
class AntTweakBarSystem;
class InputBackendSystem;
// =======================================================================================
// AntTweakBarEnablerSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief Brief...
///        
/// # AntTweakBarEnablerSystem
/// Detailed description...
/// Created on: 12-2-2013 
///---------------------------------------------------------------------------------------
class AntTweakBarEnablerSystem: public EntitySystem
{
public:
	AntTweakBarEnablerSystem();
	void initialize();
	void process();

private:
	AntTweakBarSystem* m_antSystem;
	InputBackendSystem* m_inputBackend;
};