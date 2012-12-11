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

		UINT32 flags = 0;
	
		flags |= XAUDIO2_DEBUG_ENGINE;

		hr = XAudio2Create( &newAudioEngine, flags );
		if( FAILED( hr ) ) {
			throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);
		}
		return newAudioEngine;
	}
	static IXAudio2MasteringVoice* createMasterVoice(IXAudio2* p_engine)
	{
		HRESULT hr = S_OK;
		IXAudio2MasteringVoice* masterVoice;
	
		UINT32 dCount = 0;
		hr = p_engine->GetDeviceCount( &dCount );
		if (FAILED(hr) )
			throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

		for( UINT32 index=0; index < dCount; ++index )
		{
			XAUDIO2_DEVICE_DETAILS details;
			hr = p_engine->GetDeviceDetails( index, &details );
			if (FAILED(hr))
				throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

			// display name is details.DisplayName
			// internal device ID is details.DeviceID
			// the 'index' is used for creating the mastering voice
		}
	
		//UINT32 deviceIndex;
		hr = p_engine->CreateMasteringVoice( &masterVoice); /*, 
			XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, deviceIndex, NULL );*/

		return masterVoice;
	}

	static void createXAudio3(IXAudio2* p_audioEngine, X3DAUDIO_HANDLE p_xaudio3d)
	{
		HRESULT hr = S_OK;

		XAUDIO2_DEVICE_DETAILS details;
		hr = p_audioEngine->GetDeviceDetails( 0, &details );
		if ( FAILED(hr) )
			throw XAudio2Exception(hr,__FILE__,__FUNCTION__,__LINE__);

		DWORD dwChannelMask = details.OutputFormat.dwChannelMask;
		UINT32 nChannels = details.OutputFormat.Format.nChannels;

		X3DAudioInitialize( dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, p_xaudio3d );
	}
}