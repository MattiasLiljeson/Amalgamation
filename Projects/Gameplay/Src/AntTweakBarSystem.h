#pragma once
#include <EntitySystem.h>

class GraphicsBackendSystem;
class InputBackendSystem;

// =======================================================================================
//                                      AntTweakBarSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AntTweakBarSystem
/// Detailed description.....
/// Created on: 15-1-2013 
///---------------------------------------------------------------------------------------

class AntTweakBarSystem : public EntitySystem
{
public:
	AntTweakBarSystem( GraphicsBackendSystem* p_gfxBackend,
		InputBackendSystem* p_inputBackend );
	~AntTweakBarSystem();
};

