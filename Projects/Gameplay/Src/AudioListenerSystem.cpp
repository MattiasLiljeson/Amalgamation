#include "AudioListenerSystem.h"

AudioListenerSystem::AudioListenerSystem(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::AudioListenerSystem, 2, ComponentType::Transform, 
	ComponentType::CameraInfo)
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
		CameraInfo* camerInfo = static_cast<CameraInfo*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::CameraInfo ) );

		Transform* translationInfo = static_cast<Transform*>(
			p_entities[0]->getComponent( ComponentType::ComponentTypeIdx::Transform ) );

		listenerInfo.listenerPos = translationInfo->getTranslation();
		listenerInfo.listenerOrientFront = AglVector3::createDirection(camerInfo->m_lookAt,
			translationInfo->getTranslation());
		listenerInfo.listenerOrientTop	= camerInfo->m_up;

		/************************************************************************/
		/* HACK:!!!THERE IS NO VELOCITY!!!										*/
		/************************************************************************/
		listenerInfo.listenerVelocity	= AglVector3(0,0,0);

		m_audioBackend->updateListener(listenerInfo);
	}
}
