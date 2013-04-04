#pragma once
#include "EntitySystem.h"

// =======================================================================================
//                                      FadeInSystem
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Handles the fading in and fading out of Audio
///        
/// # FadeInSystem
/// Detailed description.....
/// Created on: 22-3-2013 
///---------------------------------------------------------------------------------------

class FadeInSystem : public EntitySystem
{
public:
	FadeInSystem();
	~FadeInSystem();
	void processEntities( const vector<Entity*>& p_entities );
private:
	bool m_fading;
	float m_previousVolume;
	float m_fadingTime;
	static float FADINGSPEED;
};