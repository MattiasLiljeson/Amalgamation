#include "SkeletalAnimation.h"

ComponentRegister<SkeletalAnimation> SkeletalAnimation::s_reg("SkeletalAnimation");

SkeletalAnimation::SkeletalAnimation() : Component(ComponentType::SkeletalAnimation)
{
	m_time = 0;
	m_scene = NULL;
	m_offset = AglMatrix::identityMatrix();
	m_isPlaying = false;
	m_fps = 1.0f;
}

SkeletalAnimation::SkeletalAnimation( float p_time, AglScene* p_scene, AglMatrix p_offset ) : Component(ComponentType::SkeletalAnimation)
{
	m_time = p_time;
	m_scene = p_scene;
	m_offset = p_offset;
	m_isPlaying = false;
	m_fps = 1.0f;
	m_currentTake = -1;
}

SkeletalAnimation::~SkeletalAnimation()
{

}

void SkeletalAnimation::init( vector<ComponentData> p_initData )
{
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
		string name = p_initData[i].dataName;
		if(name == "m_fps")
		{ 
			p_initData[i].getData<float>(&m_fps);
		}
		else if (name == "m_name")
		{
			m_takes.push_back(AnimationTake());
			p_initData[i].getDataAsString(&m_takes.back().name);
		}
		else if (name == "m_startFrame")
		{
			p_initData[i].getData<int>(&m_takes.back().startFrame);
		}
		else if (name == "m_endFrame")
		{
			p_initData[i].getData<int>(&m_takes.back().endFrame);
		}
	}
	m_time = 0;
	m_isPlaying = false;
	m_scene = NULL;
	m_offset = AglMatrix::identityMatrix();
	m_currentTake = 0;
}
