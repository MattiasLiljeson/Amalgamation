#include "AglNodeAnimation.h"
#include "AglScene.h"

AglNodeAnimation::AglNodeAnimation(AglNodeAnimationHeader p_header, AglKeyFrame* p_keyFrames, AglScene* p_scene)
{
	m_header = p_header;
	m_keyFrames = p_keyFrames;
	m_lastEvaluated = 0;
	m_scene = p_scene;
}
AglNodeAnimation::~AglNodeAnimation()
{
	delete[] m_keyFrames;
}

AglNodeAnimationHeader AglNodeAnimation::getHeader()
{
	return m_header;
}
AglKeyFrame* AglNodeAnimation::getKeyFrames()
{
	return m_keyFrames;
}

void AglNodeAnimation::appendTransform(float p_time, float p_weight)
{
	if (p_time < m_keyFrames[m_lastEvaluated].time)
		m_lastEvaluated = 0;
	while (m_lastEvaluated < m_header.keyFrameCount - 1 && p_time > m_keyFrames[m_lastEvaluated].time)
	{
		m_lastEvaluated++;
	}

	if (m_lastEvaluated == 0)
	{
		m_scene->appendTransform(m_header.targetNode, m_keyFrames[m_lastEvaluated].transform);
	}
	else
	{
		float frac = (p_time - m_keyFrames[m_lastEvaluated-1].time) / (m_keyFrames[m_lastEvaluated].time - m_keyFrames[m_lastEvaluated-1].time);

		AglMatrix m1 = m_keyFrames[m_lastEvaluated-1].transform;
		AglMatrix m2 = m_keyFrames[m_lastEvaluated].transform;
		AglMatrix transform = m1*(1-frac) + m2*frac;
	
		m_scene->appendTransform(m_header.targetNode, transform);
	}
}
float AglNodeAnimation::getMinTime()
{
	return m_keyFrames[0].time;
}
float AglNodeAnimation::getMaxTime()
{
	return m_keyFrames[m_header.keyFrameCount-1].time;
}