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
#include <deque>

using namespace std;

class AglScene;

struct AnimationTake
{
	string name;
	int startFrame;
	int endFrame;
	float speed;

	AnimationTake()
	{
		name = "";
		startFrame = 0;
		endFrame = 0;
		speed = 1.0f;
	}
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
	float m_fps;
	int m_currentTake;
	deque<int> queuedTakes;

	vector<AnimationTake> m_takes;
	static ComponentRegister<SkeletalAnimation> s_reg;
};