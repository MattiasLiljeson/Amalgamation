#include "AudioListenerSystem.h"
#include "AudioBackendSystem.h"
#include "AudioListener.h"
#include "Transform.h"
#include "SoundOrientation.h"

AudioListenerSystem::AudioListenerSystem(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::AudioListenerSystem, 3, ComponentType::TAG_MainCamera,
	ComponentType::Transform, ComponentType::AudioListener)
{
	m_audioBackend = p_audioBackend;
	m_listener = SoundOrientation();
}

AudioListenerSystem::~AudioListenerSystem()
{

}

void AudioListenerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(!p_entities.empty())
	{
		AudioListener* audioListener = static_cast<AudioListener*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::AudioListener));

		Transform* translationInfo = static_cast<Transform*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		m_listener.listenerPos			= translationInfo->getTranslation();
		m_listener.listenerOrientFront	= translationInfo->getMatrix().GetForward();
		m_listener.listenerOrientTop		= translationInfo->getMatrix().GetUp();

		/************************************************************************/
		/* HACK:!!!THERE IS NO VELOCITY!!!										*/
		/************************************************************************/
		m_listener.listenerVelocity	= AglVector3(0,0,0);

		m_audioBackend->updateListener( m_listener );
		m_audioBackend->updateListenerVolume( audioListener->getListenerVolume() );
	}
}

SoundOrientation* AudioListenerSystem::getListenerOrientation()
{
	return &m_listener;
}
