#include "FadeInSystem.h"
#include "ClientStateSystem.h"
#include "AudioBackendSystem.h"
#include "AudioListener.h"
#include <ValueClamp.h>

float FadeInSystem::FADINGSPEED = 10.0f;

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
			m_fadingTime = 0.0f;
		}

		if(m_fading){
			m_fadingTime += (m_world->getDelta());
			float newMasterVolume = m_fadingTime/FADINGSPEED;
			audioListener->setListenerVolume(newMasterVolume);
			if(m_fadingTime>=FADINGSPEED){
				audioListener->setListenerVolume(m_previousVolume);
				m_fading = false;
			}
		}
	}
}