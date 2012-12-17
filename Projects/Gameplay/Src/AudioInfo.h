#pragma once

#include "Component.h"

// =======================================================================================
//                                      AudioInfo
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # AudioInfo
/// Detailed description.....
/// Created on: 13-12-2012 
///---------------------------------------------------------------------------------------

class AudioInfo : public Component
{
public:
	AudioInfo(int p_soundId, bool p_isPositionalSound);
	~AudioInfo();
	int getSoundId() const;
	bool isPositionalSound() const;
private:
	int m_soundId;
	bool m_isPositionalSound;
};