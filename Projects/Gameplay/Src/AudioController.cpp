#include "AudioController.h"

AudioController::AudioController(AudioBackendSystem* p_audioBackend) : 
	EntitySystem(SystemType::AudioControllerSystem, 1, ComponentType::AudioComponent)
{
	m_audioBackend = p_audioBackend;
}

AudioController::~AudioController()
{

}

void AudioController::initialize()
{

}

void AudioController::processEntities( const vector<Entity*>& p_entities )
{
	if(!p_entities.empty())
	{
		for (unsigned int i = 0; i < p_entities.size(); i++)
		{
			AudioInfo* comp; 
			comp = static_cast<AudioInfo*>(p_entities[i]->getComponent(
				ComponentType::AudioComponent));
			if(comp->isPositionalSound())
				m_audioBackend->getSoundWrapper()->updateOutputMatrix(comp->getSoundId());
		}
	}
}


