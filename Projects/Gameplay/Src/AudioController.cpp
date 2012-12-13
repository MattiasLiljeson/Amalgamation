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
	m_basicSound = m_audioBackend->createNonPositionalSound(
		"Assets/Sound/Music/Test/Techno_1.wav");
	m_basicPositionalSound = m_audioBackend->createPositionalSound(
		"Assets/Sound/Music/Test/MusicMono.wav", AglVector3(0,0,0));

	m_audioBackend->changeAudioInstruction(m_basicSound,SoundEnums::Instructions::PLAY);
}

void AudioController::processEntities( const vector<Entity*>& p_entities )
{
	if(!p_entities.empty())
	{
		/************************************************************************/
		/* DO STUFF!! */
		/************************************************************************/
	}
}


