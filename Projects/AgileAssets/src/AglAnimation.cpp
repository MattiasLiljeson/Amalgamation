#include "AglAnimation.h"
#include "AglScene.h"

void AglAnimation::determineTimeLimits()
{
	m_maxTime = m_scene->getAnimationLayer(m_layers[0])->getMaxTime();
	m_minTime = m_currentTime = m_scene->getAnimationLayer(m_layers[0])->getMinTime();
	for (unsigned int i = 1; i < m_header.layerCount; i++)
	{
		if (m_scene->getAnimationLayer(m_layers[i])->getMaxTime() > m_maxTime)
			m_maxTime = m_scene->getAnimationLayer(m_layers[i])->getMaxTime();
		if (m_scene->getAnimationLayer(m_layers[i])->getMinTime() < m_minTime)
			m_minTime = m_currentTime = m_scene->getAnimationLayer(m_layers[i])->getMinTime();
	}
	m_limitsSet = true;
}

AglAnimation::AglAnimation(AglAnimationHeader p_header, unsigned int* p_layers, AglScene* p_scene)
{
	m_header = p_header;
	m_layers = p_layers;
	m_isPlaying = false;
	m_currentTime = 0.0f;

	m_limitsSet = false;
	m_maxTime = -1;
	m_minTime = -1;
	m_scene = p_scene;
}
AglAnimation::~AglAnimation()
{
	delete[] m_layers;
}
AglAnimationHeader AglAnimation::getHeader()
{
	return m_header;
}
unsigned int* AglAnimation::getLayers()
{
	return m_layers;
}
void AglAnimation::update(float p_dt)
{
	if (m_isPlaying)
	{
		if (!m_limitsSet)
			determineTimeLimits();

		m_currentTime += p_dt;

		while (m_currentTime > m_maxTime)
			m_currentTime -= (m_maxTime - m_minTime);
		while (m_currentTime < m_minTime)
			m_currentTime += (m_maxTime - m_minTime);

		for (unsigned int i = 0; i < m_header.layerCount; i++)
		{
			AglAnimationLayer* l = m_scene->getAnimationLayer(m_layers[i]);
			l->evaluate(m_currentTime);
		}
	}
}
void AglAnimation::play()
{
	m_isPlaying = true;
	if (!m_limitsSet)
		determineTimeLimits();
	m_currentTime = m_minTime;
}