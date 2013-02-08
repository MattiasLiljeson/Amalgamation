#include "AglGradient.h"

AglGradient::AglGradient()
{
}
AglGradient::~AglGradient()
{
	for (unsigned int i = 0; i < m_layers.size(); i++)
		delete m_layers[i];
}
vector<AglGradientMaterial*> AglGradient::getLayers()
{
	return m_layers;
}
void AglGradient::addLayer(AglGradientMaterial* p_layer)
{
	m_layers.push_back(p_layer);
}
void AglGradient::removeLayer(AglGradientMaterial* p_layer)
{
	for (unsigned int i = 0; i < m_layers.size(); i++)
	{
		if (p_layer == m_layers[i])
		{
			delete m_layers[i];
			m_layers[i] = m_layers.back();
			m_layers.pop_back();
			return;
		}
	}
}