#include "SkeletalAnimation.h"

ComponentRegister<SkeletalAnimation> SkeletalAnimation::s_reg("SkeletalAnimation");

SkeletalAnimation::SkeletalAnimation() : Component(ComponentType::SkeletalAnimation)
{
	m_time = 0;
	m_scene = NULL;
	m_offset = AglMatrix::identityMatrix();
	m_isPlaying = false;
	m_playSpeed = 1.0f;
}

SkeletalAnimation::SkeletalAnimation( float p_time, AglScene* p_scene, AglMatrix p_offset ) : Component(ComponentType::SkeletalAnimation)
{
	m_time = p_time;
	m_scene = p_scene;
	m_offset = p_offset;
	m_isPlaying = false;
	m_playSpeed = 1.0f;
}

SkeletalAnimation::~SkeletalAnimation()
{

}

void SkeletalAnimation::init( vector<ComponentData> p_initData )
{
	for( unsigned int i=0; i<p_initData.size(); i++ )
	{
	}
	m_time = 0;
	m_isPlaying = false;
	m_scene = NULL;
	m_playSpeed = 0;
	m_offset = AglMatrix::identityMatrix();
}
