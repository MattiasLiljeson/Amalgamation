#include "AntTweakBarSystem.h"
#include "GraphicsBackendSystem.h"
#include "InputBackendSystem.h"


AntTweakBarSystem::AntTweakBarSystem( GraphicsBackendSystem* p_gfxBackend,
									 InputBackendSystem* p_inputBackend )
									 : EntitySystem( SystemType::AntTweakBarSystem )
{
}


AntTweakBarSystem::~AntTweakBarSystem(void)
{
}
