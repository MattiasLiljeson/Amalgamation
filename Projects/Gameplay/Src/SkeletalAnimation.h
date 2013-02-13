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

class AglScene;

class SkeletalAnimation : public Component
{
public:
	SkeletalAnimation(float p_time, AglScene* p_scene) : Component(ComponentType::SkeletalAnimation)
	{
		m_time = p_time;
		m_scene = p_scene;
	}
	virtual ~SkeletalAnimation()
	{

	}

	float m_time;
	AglScene* m_scene;
};