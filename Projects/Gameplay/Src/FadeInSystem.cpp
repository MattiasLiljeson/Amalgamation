#include "FadeInSystem.h"
#include "ClientStateSystem.h"
#include "AudioBackendSystem.h"
#include "AudioListener.h"

FadeInSystem::FadeInSystem() : EntitySystem(SystemType::FadeInSystem, 1, 
											ComponentType::AudioListener)
{
	m_fading = false;
}

FadeInSystem::~FadeInSystem()
{

}

void FadeInSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(p_entities.size()>0){
		ClientStateSystem* gameState = static_cast<ClientStateSystem*>(
			m_world->getSystem(SystemType::ClientStateSystem));

		AudioListener* audioListener = static_cast<AudioListener*>(
			p_entities[0]->getComponent( ComponentType::AudioListener));

		if(gameState->getStateDelta(GameStates::LOBBY) == EnumGameDelta::EXITTHISFRAME){
			AudioBackendSystem* backEnd = static_cast<AudioBackendSystem*>
				(m_world->getSystem(SystemType::AudioBackendSystem));

			m_previousVolume = audioListener->getListenerVolume();
			audioListener->setListenerVolume(0);
		}

		if(gameState->getStateDelta(GameStates::INGAME) == EnumGameDelta::ENTEREDTHISFRAME){
			m_fading = true;
		}

		if(m_fading){
			audioListener->setListenerVolume( m_world->getDelta()*m_world->getDelta()*m_world->getDelta()
				+ audioListener->getListenerVolume() );
			AudioBackendSystem* backEnd = static_cast<AudioBackendSystem*>
				(m_world->getSystem(SystemType::AudioBackendSystem));
			if(audioListener->getListenerVolume()>m_previousVolume){
				audioListener->setListenerVolume(m_previousVolume);
				m_fading = false;
			}
		}
	}
}