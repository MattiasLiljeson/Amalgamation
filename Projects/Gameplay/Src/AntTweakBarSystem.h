#pragma once
#include <EntitySystem.h>
#include <RenderInterface.h>

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

class AntTweakBarSystem : public EntitySystem, public RenderInterface
{
public:
	AntTweakBarSystem( GraphicsBackendSystem* p_gfxBackend,
		InputBackendSystem* p_inputBackend );
	~AntTweakBarSystem();

	virtual void process();

	void feedInput();

	virtual void render();


private:
	GraphicsBackendSystem* m_gfxBackend;
	InputBackendSystem* m_inputBackend;
};

