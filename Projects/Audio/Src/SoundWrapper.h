#pragma once

#include <AglVector3.h>
#include <vector>
#include "XAudio2Exception.h"
#include "SoundEnums.h"
#include "SoundOrientation.h"
#include "AudioCurves.h"
#include "Sound.h"
#include "PositionalSound.h"
#include "SoundFactory.h"

using namespace std;

// =======================================================================================
//                                      SoundWrapper
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Wraps the sound functionality to become easier to use
///        
/// # SoundWrapper
/// Listener position will always be initialized to 
/// Position	(0,0,0)
/// OrientTop	(0,1,0)
/// OrientFront (0,0,1)
/// Velocity	(0,0,0)
/// Created on: 6-12-2012 
///---------------------------------------------------------------------------------------

class SoundWrapper
{
public:
	///-----------------------------------------------------------------------------------
	/// Sound wrapper is not dependent of any other classes at creation.
	/// \return 
	///-----------------------------------------------------------------------------------
	SoundWrapper();

	///-----------------------------------------------------------------------------------
	/// Deletes all of the created sounds, destroys the master voice, stops the engine and
	/// in Microsoft fashion releases the device.
	/// \return 
	///-----------------------------------------------------------------------------------
	virtual ~SoundWrapper();
	///-----------------------------------------------------------------------------------
	/// Updates the listeners attributes, position, front, top and velocity.
	/// \param p_sceneInfo
	/// \return void
	///-----------------------------------------------------------------------------------
	void updateListener(const SoundOrientation& p_sceneInfo);

	///-----------------------------------------------------------------------------------
	/// Utilizes the sound factory class to create the new non positional sound. The 
	/// created sound is then stored for easy access through the ES system.
	/// \param p_filePath
	/// \return int
	///-----------------------------------------------------------------------------------
	int createNewNonPositionalSound(const char* p_filePath);

	///-----------------------------------------------------------------------------------
	/// Utilizes the sound factory class to create the new positional sound. The created
	/// sound is then stored for easy access through the ES system.
	/// \param p_filePath
	/// \param p_pos
	/// \return int
	///-----------------------------------------------------------------------------------
	int createNewPositionalSound(const char* p_filePath, const AglVector3& p_pos);

	///-----------------------------------------------------------------------------------
	/// To update positional sound output matrix is a relative expensive function to be 
	/// called, where it calculates the 3D positional sound output matrix for a given sound.
	/// \param p_index
	/// \return void
	///-----------------------------------------------------------------------------------
	void updateOutputMatrix(int p_index);

	///-----------------------------------------------------------------------------------
	/// Updates
	/// \param p_index
	/// \param p_soundInstruction
	/// \return void
	///-----------------------------------------------------------------------------------
	void updateSound(int p_index, const SoundEnums::Instructions& p_soundInstruction);

	///-----------------------------------------------------------------------------------
	/// Returns a reference to the master volume used to alter the master volume.
	/// \return float*
	///-----------------------------------------------------------------------------------
	float* getMasterVolumeRef();

	///-----------------------------------------------------------------------------------
	/// Returns the master volume
	/// \return float
	///-----------------------------------------------------------------------------------
	float getMasterVolume() const;

	///-----------------------------------------------------------------------------------
	/// Sets the master volume value but doesn't update the it, call updateMasterVolume 
	/// instead.
	/// \param p_value
	/// \return void
	///-----------------------------------------------------------------------------------
	void setMasterVolume(const float p_value);

	///-----------------------------------------------------------------------------------
	/// Is called to set the new master volume.
	/// \return void
	///-----------------------------------------------------------------------------------
	void updateMasterVolume();

	float* getLeftChannelRef();
	float* getRightChannelRef();

private:
	///-----------------------------------------------------------------------------------
	/// Initialize the XAudio2 parts of the sound engine. If debug is used when compiling
	/// XAudio2 debug engine is set.
	/// \return void
	///-----------------------------------------------------------------------------------
	void initSoundEngine();

	///-----------------------------------------------------------------------------------
	/// Initialize the X3DAudio parts of the sound engine.
	/// \return void
	///-----------------------------------------------------------------------------------
	void init3DSoundSettings();

	///-----------------------------------------------------------------------------------
	/// Initialize the listener to default values, see detailed description of the class
	/// \return void
	///-----------------------------------------------------------------------------------
	void initListener();
private:
	const static int SOURCECHANNELS = 1;
	IXAudio2* m_soundDevice;
	IXAudio2MasteringVoice* m_masterVoice;

	float m_masterVolume;
	UINT32 m_destChannels;
	DWORD m_channelMask;

	XAUDIO2_DEVICE_DETAILS m_details;

	/************************************************************************/
	/* 3D Audio needed information											*/
	/************************************************************************/
	X3DAUDIO_LISTENER		m_listener;
	X3DAUDIO_HANDLE			m_x3DAudioInstance;
	X3DAUDIO_DSP_SETTINGS	m_dspSettings;

	FLOAT32*				m_matrixCoefficients;
	FLOAT32*				m_emitterAzimuths;

	float					m_left;
	float					m_right;

	SoundFactory*			m_soundFactory;

	vector<Sound*>			m_createdSounds;
};