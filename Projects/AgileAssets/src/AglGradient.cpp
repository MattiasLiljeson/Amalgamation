#include "AglGradient.h"

AglGradient::AglGradient()
{
	m_layers.push_back(AglGradientMaterial());
}
AglGradient::~AglGradient()
{
}
vector<AglGradientMaterial> AglGradient::getLayers()
{
	return m_layers;
}
void AglGradient::addLayer(AglGradientMaterial p_layer)
{
	m_layers.push_back(p_layer);
}