// =======================================================================================
//                                      Skeletal Animation
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Brief
///        
/// # Skeletal Animation
/// Detailed description.....
/// Created on: 13-02-2013 
///---------------------------------------------------------------------------------------
#pragma once
#include <Component.h>
#include <AglMatrix.h>

class AglScene;

class SkeletalAnimation : public Component
{
public:
	SkeletalAnimation(float p_time, AglScene* p_scene, AglMatrix p_offset) : Component(ComponentType::SkeletalAnimation)
	{
		m_time = p_time;
		m_scene = p_scene;
		m_offset = p_offset;
		m_isPlaying = false;
		m_playSpeed = 1.0f;
	}
	virtual ~SkeletalAnimation()
	{

	}

	float m_time;
	AglScene* m_scene;
	AglMatrix m_offset;
	bool m_isPlaying;
	float m_playSpeed;
};