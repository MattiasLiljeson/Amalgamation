#pragma once
#include "Component.h"

// =======================================================================================
//                                      AudioListener
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Defines the audio listener in the game world
///        
/// # AudioListener
/// Detailed description.....
/// Created on: 18-12-2012 
///---------------------------------------------------------------------------------------

class AudioListener : public Component
{
public:
	AudioListener(float p_listenerVolume=1.0f);
	~AudioListener(){};
	float* getMasterVolumeRef();
	float getListenerVolume() const;
	void setListenerVolume( float p_volume );
private:
	float m_listenerVolume;
};