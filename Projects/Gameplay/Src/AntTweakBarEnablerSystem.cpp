#include "AntTweakBarEnablerSystem.h"
#include "AntTweakBarSystem.h"
#include "InputBackendSystem.h"

AntTweakBarEnablerSystem::AntTweakBarEnablerSystem()
	: EntitySystem( SystemType::AntTweakBarEnablerSystem )
{
}

void AntTweakBarEnablerSystem::initialize()
{
	m_antSystem = static_cast<AntTweakBarSystem*>(m_world->getSystem(
		SystemType::AntTweakBarSystem));
	m_inputBackend = static_cast<InputBackendSystem*>(m_world->getSystem(
		SystemType::InputBackendSystem));
}

void AntTweakBarEnablerSystem::process()
{
	if(m_inputBackend->getDeltaByEnum(InputHelper::KeyboardKeys_NUMPAD_3) > 0)
	{
		m_antSystem->setEnabled( !m_antSystem->getEnabled() );
	}
}