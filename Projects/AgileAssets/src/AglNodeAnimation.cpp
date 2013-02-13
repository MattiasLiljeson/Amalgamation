#include "AglNodeAnimation.h"
#include "AglScene.h"

AglNodeAnimation::AglNodeAnimation(AglNodeAnimationHeader p_header, AglKeyFrame* p_keyFrames, AglScene* p_scene)
{
	m_header = p_header;
	m_keyFrames = p_keyFrames;
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
	if (p_time <= m_keyFrames[0].time)
		m_scene->appendTransform(m_header.targetNode, m_keyFrames[0].transform);
	else if (p_time > m_keyFrames[m_header.keyFrameCount-1].time)
		m_scene->appendTransform(m_header.targetNode, m_keyFrames[m_header.keyFrameCount-1].transform);
	else
	{
		unsigned int pos = 0;
		while (pos < m_header.keyFrameCount-2 && m_keyFrames[pos+1].time < p_time)
		{
			pos++;
		}

		unsigned int pos2 = pos+1;

		float frac = (p_time - m_keyFrames[pos].time) / (m_keyFrames[pos2].time - m_keyFrames[pos].time);

		AglMatrix m1 = m_keyFrames[pos].transform;
		AglMatrix m2 = m_keyFrames[pos2].transform;
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