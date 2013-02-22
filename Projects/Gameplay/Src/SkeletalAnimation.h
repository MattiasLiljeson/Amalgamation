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
#include <ComponentFactory.h>
#include <vector>

using namespace std;

class AglScene;

struct AnimationTake
{
	float speed;
	int startFrame;
	int endFrame;
};

class SkeletalAnimation : public Component
{
public:
	SkeletalAnimation();
	SkeletalAnimation(float p_time, AglScene* p_scene, AglMatrix p_offset);
	virtual ~SkeletalAnimation();
	void init( vector<ComponentData> p_initData );

	float m_time;
	AglScene* m_scene;
	AglMatrix m_offset;
	bool m_isPlaying;
	float m_playSpeed;

	vector<AnimationTake> m_takes;
	static ComponentRegister<SkeletalAnimation> s_reg;
};