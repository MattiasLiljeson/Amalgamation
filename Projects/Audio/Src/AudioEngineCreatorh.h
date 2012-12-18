#include <strsafe.h>
#include <Windows.h>
#include <xaudio2.h>
#include <X3DAudio.h>
#include "XAudio2Exception.h"

namespace AudioEngineCreator
{
	static IXAudio2* createAudioEngine()
	{
		HRESULT hr = S_OK;

		IXAudio2* newAudioEngine;

		/************************************************************************/
		/* Check if we are compiling for debug or not, use debug engine or not. */
		/************************************************************************/


		hr = XAudio2Create( &newAudioEngine, flags );
		if( FAILED( hr ) ) {
			throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);
		}
		return newAudioEngine;
	}

	static IXAudio2MasteringVoice* createMasterVoice(IXAudio2* p_audioEngine)
	{
		HRESULT hr = S_OK;
		IXAudio2MasteringVoice* masterVoice;
	
		/************************************************************************/
		/* Maybe looping through all the different sound devices might not be	*/
		/* something we would like to do? Right now we select the primary device*/
		/* regardless of what other devices there might be.						*/
		/************************************************************************/
		/* BEGIN */
		UINT32 dCount = 0;
		hr = p_audioEngine->GetDeviceCount( &dCount );
		if (FAILED(hr) )
			throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

		for( UINT32 index=0; index < dCount; ++index )
		{
			XAUDIO2_DEVICE_DETAILS details;
			hr = p_audioEngine->GetDeviceDetails( index, &details );
			if (FAILED(hr))
				throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

			// display name is details.DisplayName
			// internal device ID is details.DeviceID
			// the 'index' is used for creating the mastering voice
		}
		/* END */
	
		/*************************************************************************/
		/* The creation of the mastervoice will have to set to default right now.*/
		/*************************************************************************/
		hr = p_audioEngine->CreateMasteringVoice( &masterVoice); /*, 
			XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, deviceIndex, NULL );*/

		return masterVoice;
	}
}