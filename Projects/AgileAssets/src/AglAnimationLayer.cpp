#include "AglAnimationLayer.h"
#include "AglScene.h"

AglAnimationLayer::AglAnimationLayer(AglAnimationLayerHeader p_header, unsigned int* p_nodes, AglScene* p_scene)
{
	m_header = p_header;
	m_nodes = p_nodes;
	m_scene = p_scene;
}
AglAnimationLayer::~AglAnimationLayer()
{
	delete[] m_nodes;
}

AglAnimationLayerHeader AglAnimationLayer::getHeader()
{
	return m_header;
}
unsigned int* AglAnimationLayer::getNodes()
{
	return m_nodes;
}

void AglAnimationLayer::evaluate(float p_time)
{
	//Right now only weights of 100% are supported. Must add functionality to support dynamic weights
	for (unsigned int i = 0; i < m_header.nodeCount; i++)
	{
		AglNodeAnimation* nodeAnimation = m_scene->getNodeAnimation(m_nodes[i]);
		nodeAnimation->appendTransform(p_time, 1.0f);
	}
}
AglMatrix AglAnimationLayer::evaluate(float p_time, unsigned int p_node)
{
	AglNodeAnimation* nodeAnimation = m_scene->getNodeAnimation(m_nodes[p_node]);
	return nodeAnimation->evaluate(p_time);
}

float AglAnimationLayer::getMinTime()
{
	if (m_header.nodeCount == 0)
		return 0;
	float min = m_scene->getNodeAnimation(m_nodes[0])->getMinTime();
	for (unsigned int i = 1; i < m_header.nodeCount ; i++)
	{
		if (m_scene->getNodeAnimation(m_nodes[i])->getMinTime() < min)
			min = m_scene->getNodeAnimation(m_nodes[i])->getMinTime();
	}
	return min;
}
float AglAnimationLayer::getMaxTime()
{
	if (m_header.nodeCount == 0)
		return 1000;
	float max = m_scene->getNodeAnimation(m_nodes[0])->getMaxTime();
	for (unsigned int i = 1; i < m_header.nodeCount; i++)
	{
		if (m_scene->getNodeAnimation(m_nodes[i])->getMaxTime() > max)
			max = m_scene->getNodeAnimation(m_nodes[i])->getMaxTime();
	}
	return max;
}