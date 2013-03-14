#include "AudioListenerSystem.h"
#include "AudioBackendSystem.h"
#include "AudioListener.h"
#include "Transform.h"
#include "SoundOrientation.h"
#include "MeshOffsetTransform.h"

AudioListenerSystem::AudioListenerSystem(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::AudioListenerSystem, 2,
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
			p_entities[0]->getComponent( ComponentType::AudioListener));

		Transform* trans = static_cast<Transform*>(
			p_entities[0]->getComponent( ComponentType::Transform ) );

		MeshOffsetTransform* offsetTrans = static_cast<MeshOffsetTransform*>(
			p_entities[0]->getComponent( ComponentType::MeshOffsetTransform ));

		if(offsetTrans){
			AglMatrix worldTransform = offsetTrans->offset.inverse()*trans->getMatrix();

			m_listener.listenerOrientFront	= worldTransform.GetForward();
			m_listener.listenerOrientTop	= worldTransform.GetUp();
			m_listener.listenerPos			= worldTransform.GetTranslation();
		}
		else{
			m_listener.listenerPos			= trans->getTranslation();
			m_listener.listenerOrientFront	= trans->getMatrix().GetForward();
			m_listener.listenerOrientTop	= trans->getMatrix().GetUp();
		}

		/************************************************************************/
		/* HACK:!!!THERE IS NO VELOCITY!!!										*/
		/************************************************************************/
		m_listener.listenerVelocity	= AglVector3::zero();


		m_audioBackend->updateListener( m_listener );
		m_audioBackend->updateListenerVolume( audioListener->getListenerVolume() );
	}
}

SoundOrientation* AudioListenerSystem::getListenerOrientation()
{
	return &m_listener;
}

void AudioListenerSystem::initialize()
{
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::OVERALL,
		"ListenerPosX", TwType::TW_TYPE_FLOAT, &m_listener.listenerPos.x, "group=Ship");
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::OVERALL,
		"ListenerPosY", TwType::TW_TYPE_FLOAT, &m_listener.listenerPos.y, "group=Ship");
	AntTweakBarWrapper::getInstance()->addReadOnlyVariable(AntTweakBarWrapper::OVERALL,
		"ListenerPosZ", TwType::TW_TYPE_FLOAT, &m_listener.listenerPos.z, "group=Ship");
}
