#include "AudioListenerSystem.h"
#include "AudioBackendSystem.h"
#include "AudioListener.h"
#include "Transform.h"
#include "SoundOrientation.h"

AudioListenerSystem::AudioListenerSystem(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::AudioListenerSystem, 2, ComponentType::Transform, 
	ComponentType::AudioListener)
{
	m_audioBackend = p_audioBackend;
}

AudioListenerSystem::~AudioListenerSystem()
{

}

void AudioListenerSystem::processEntities( const vector<Entity*>& p_entities )
{
	if(!p_entities.empty())
	{
		SoundOrientation listenerInfo;
		AudioListener* audioListener = static_cast<AudioListener*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::AudioListener ) );

		Transform* translationInfo = static_cast<Transform*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		listenerInfo.listenerPos			= translationInfo->getTranslation();
		listenerInfo.listenerOrientFront	= translationInfo->getMatrix().GetForward();
		listenerInfo.listenerOrientTop		= translationInfo->getMatrix().GetUp();

		/************************************************************************/
		/* HACK:!!!THERE IS NO VELOCITY!!!										*/
		/************************************************************************/
		listenerInfo.listenerVelocity	= AglVector3(0,0,0);

		m_audioBackend->updateListener( listenerInfo );
		m_audioBackend->updateListenerVolume( audioListener->getListenerVolume() );
	}
}